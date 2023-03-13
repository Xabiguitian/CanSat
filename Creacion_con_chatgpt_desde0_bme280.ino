#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
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

float temperature;
float pressure;
float humidity;

Adafruit_BME280 bme;

void setup() {
  Serial.begin(9600); // Inicializar la comunicación serial
  while (!Serial);

  SPI.begin(SCK, MISO, MOSI, SS);  // Inicializar SPI
  LoRa.setPins(SS, RST, DIO0);     // Configurar los pines del módulo LoRa32

  if (!LoRa.begin(868E6)) {        // Inicializar el módulo LoRa32 en la frecuencia 868 MHz
    Serial.println("Error al iniciar el módulo LoRa32");
    while (1);
  }

  bme.begin(0x76);  // Inicializar el sensor BME280 en la dirección 0x76

  // Inicializar la pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Error al iniciar la pantalla OLED");
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Datos del sensor BME280:");
  display.display();
}

void loop() {
  temperature = bme.readTemperature();  // Leer la temperatura del sensor BME280
  pressure = bme.readPressure() / 100.0F;  // Leer la presión del sensor BME280 y convertirla a hPa
  humidity = bme.readHumidity();  // Leer la humedad del sensor BME280

  Serial.print("Temperatura = ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Presión = ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Humedad = ");
  Serial.print(humidity);
  Serial.println(" %");

  // Crear un mensaje con los datos y enviarlo a través del módulo LoRa32
  String message = String(temperature) + "," + String(pressure) + "," + String(humidity);
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();

  // Mostrar los datos en la pantalla OLED
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("Temperatura = ");
  display.print(temperature);
  display.println(" C");
  display.print("Presion = ");
  display.print(pressure);
  display.println(" hPa");
  display.print("Humedad = ");
  display.print(humidity);
  display.println(" %");
  display.display();

  delay(5000);
}
