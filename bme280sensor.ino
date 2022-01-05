/************
 * Wiring for Pimoroni BME280 breakout board, to Arduino UNO:
 *    SDA -> SDA
 *    SCL -> SCL
 *    2-6v -> 5v
 *    GND -> GND
 */

#include <Wire.h>
#include <Adafruit_Sensor.h> // These two libraries need to be installed through the manage libraries tool
#include <Adafruit_BME280.h>

// Sets sea level pressure for calculating altitude
// Remember to calibrate this before every use for accuracy
#define SEALEVELPRESSURE_HPA (1018)

// Sets up I2C object
Adafruit_BME280 bme; 

void setup() {
  Serial.begin(9600);
  Serial.println("BME280 Test");
  
  unsigned status;
  status = bme.begin(0x76); // Connect to BME280 at this address
  Serial.print("BME Sensor ID = 0x");
  Serial.println(bme.sensorID(),19); // Reads the BME280 id
  Serial.println();

}

void loop() {
  printBME280Values();
  delay(1000);
}

void printBME280Values() {
    Serial.print("Temperature: ");
    Serial.print(bme.readTemperature()); // Temperature in degress celcius

    Serial.print(", Pressure: ");
    Serial.print(bme.readPressure() / 100.0F); // Pressure in hPa (hectopascals) a.k.a. milliBars

    Serial.print(", Approx-Altitude:");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    // Calculates altitude in meters above sea level, based on pressure at sea level

    Serial.print(", Humidity: ");
    Serial.println(bme.readHumidity()); // Relative (%) humidity
}
