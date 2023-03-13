#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// Configuración del módulo LoRa32
#define SCK     5     // GPIO5  -- SX1276 SCK
#define MISO    19    // GPIO19 -- SX1276 MISO
#define MOSI    27    // GPIO27 -- SX1276 MOSI
#define SS      18    // GPIO18 -- SX1276 CS
#define RST     14    // GPIO14 -- SX1276 RESET
#define DIO0    26    // GPIO26 -- SX1276 DIO0

void setup() {
  Serial.begin(9600);  // Inicializar la comunicación serial
  while (!Serial);

  SPI.begin(SCK, MISO, MOSI, SS);  // Inicializar SPI
  LoRa.setPins(SS, RST, DIO0);     // Configurar los pines del módulo LoRa32

  if (!LoRa.begin(868E6)) {        // Inicializar el módulo LoRa32 en la frecuencia 868 MHz
    Serial.println("Error al iniciar el módulo LoRa32");
    while (1);
  }

  // Inicializar la pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Error al iniciar la pantalla OLED");
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Receptor LoRa32");
  display.println("Esperando datos...");
  display.display();
}

void loop() {
  // Esperar a que se reciban datos a través del módulo LoRa32
  if (LoRa.parsePacket()) {
    String message = "";

    // Leer los datos recibidos y agregarlos a la cadena de mensaje
    while (LoRa.available()) {
      message += (char)LoRa.read();
    }

    Serial.println("Mensaje recibido: " + message);

    // Separar los datos y mostrarlos en la pantalla OLED
    float temperature = message.substring(0, message.indexOf(",")).toFloat();
    message = message.substring(message.indexOf(",") + 1);
    float pressure = message.substring(0, message.indexOf(",")).toFloat();
    message = message.substring(message.indexOf(",") + 1);
    float humidity = message.toFloat();

    display.clearDisplay();
    display.setCursor(0, 10);
    display.print("Temperatura = ");
    display.print(temperature);
    display.setCursor(0, 20);
    display.print("Presion = ");
    display.print(pressure);
    display.setCursor(0, 30);
    display.print("Humedad = ");
    display.print(humidity);
    display.display();
  }
}
