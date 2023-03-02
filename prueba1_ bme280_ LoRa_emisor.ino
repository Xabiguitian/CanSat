#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

// Definición de los pines del LoRa
#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_SS 18
#define LORA_RESET 14
#define LORA_DIO0 26

// Definición de los pines del BME280
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

Adafruit_BME280 bme; // Inicialización del sensor BME280

void setup(){
  Serial.begin(9600); // Inicialización del puerto serie
  
  // Inicialización del LoRa
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
  LoRa.setPins(LORA_SS, LORA_RESET, LORA_DIO0);
  if (!LoRa.begin(915E6)) {
    Serial.println("No se pudo inicializar el LoRa");
    while (true);
  }

  // Inicialización del BME280
  SPI.begin(BME_SCK, BME_MISO, BME_MOSI, BME_CS);
  if(!bme.begin(0x76)){
    Serial.println("No se pudo inicializar el BME280");
    while (true);
  }
  delay(1000);  
}

void loop(){
  // Lectura de los datos del sensor BME280
  float temp = bme.readTemperature();
  float hum = bme.readHumidity();
  float presion = bme.readPressure()/100;

  // Envío de los datos del sensor BME280 a través del LoRa
  LoRa.beginPacket(); // Iniciar el paquete
  LoRa.print("Temperatura: ");
  LoRa.print(temp);
  LoRa.print(" Humedad: ");
  LoRa.print(hum);
  LoRa.print(" Presion: ");
  LoRa.print(presion);
  LoRa.endPacket(); // Finalizar el paquete
  delay(2000);
}
