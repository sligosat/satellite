 /* SD card module attached to SPI bus as follows:
 *
 ** MOSI  - OI - pin 11
 ** MISO  - OO - pin 12
 ** CLK   - SCK - pin 13
 ** CS    - CS -  pin 4 
 * 
 * Pin 10 on Arduino Uno must be kept <<unused>>, in order for SD card library to work properly!!
 */

#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

// MAIN SETTINGS //
#define SEALEVELPRESSURE_HPA (1037.5) // Set up pressure at sea level 
const int SDSelectPin = 4;            // Set up your SD card CS pin here
int thermistorPin = A0;               // Sets our temperature sensor pin
int pressurePin = A1;                 // Sets our original pressure sensor pin
char fileName[] = "logFull1.txt";     // Define a filename here, max 8 characters long


// Creates I2C object
Adafruit_LIS3DH lis = Adafruit_LIS3DH();
Adafruit_BME280 bme; 

 // Sets up the SD card object
File logFile;

int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

int pressureValue;
float pressure;
float altitude;

float num1;
float num2;
float num3;
float num4;
float num5;

float ax;
float ay;
float az;
float altBME;
float tempBME;
float humBME;
float pressBME;


void setup(void) {
  Serial.begin(9600);
  
  if (!SD.begin(SDSelectPin)) { // Trys to open the SD card connection on the CS pin
    Serial.println("SD card init failed!");
    while (1) yield();
  }
  Serial.println("SD card init done.");
  logFile = SD.open(fileName, FILE_WRITE);
  logFile.println("START LOGGING");
  logFile.println("Time; press; alt; temp; aX ; aY; aZ; temp2; press2; alt2; hum;");
  logFile.close();
  
  if (! bme.begin(0x76)) { // Tries to connect to BME280 at this address
    Serial.println("BME280 init failed");
    while (1) yield();
  }
  Serial.println("BME280 init done");
  
  if (! lis.begin(0x18)) { // Stops program while trying to open connection with accelerometer over I2C
    Serial.println("LIS3DH init failed");
    while (1) yield();
  }
  Serial.println("LIS3DH init done");
  //lis.setRange(LIS3DH_RANGE_4_G);   // You can set the max acceleration range here - 2, 4, 8 or 16 G
  //lis.setDataRate(LIS3DH_DATARATE_10_HZ); // You can set the datarate to 1, 10, 25, 50, 100, 200 or 400 Hz

  Serial.println("Time; press; alt; temp; aX ; aY; aZ; temp2; press2; alt2; hum;");
}

void loop() {
  pressureReading();
  temperatureReading();
  readLIS3DH();
  readBME280();
  
  printValues();
  logValues();

  delay(1000);
}

void pressureReading() {
  pressureValue = analogRead(pressurePin);
  pressure = ((pressureValue / 1024.0) + 0.095) / 0.0009;

  num1 = pressure * 100.0;
  num2 = num1 / 101325.0;
  num3 = log10 (num2);
  num4 = (num3 / 5.25588);
  num5 = pow(10.0, num4) - 1.0;
  altitude =  num5 / -0.0000225577 + (81); // remove the 81 with better sensor
}

void temperatureReading() {
  Vo = analogRead(thermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2) + 4); // remove 4 with better sensor
  Tc = T - 273.15;
  Tf = (Tc * 9.0) / 5.0 + 32.0;
}

void readLIS3DH() {
  lis.read(); // Read the raw sensor data
  // You can access the raw data as lis.x, lis.y, lis.z
  
  sensors_event_t event; // Creates a "normalised" sensor event
  lis.getEvent(&event);

  ax = event.acceleration.x;
  ay = event.acceleration.y;
  az = event.acceleration.z;
}

void readBME280() {
    tempBME = bme.readTemperature(); // Temperature in degress celcius
    pressBME = (bme.readPressure() / 100.0F); // Pressure in hPa (hectopascals) a.k.a. milliBars
    altBME = bme.readAltitude(SEALEVELPRESSURE_HPA); // Meters above sea level
    humBME = bme.readHumidity(); // Relative (%) humidity
}

void printValues() {
  Serial.print(millis()); Serial.print("; ");

  Serial.print(pressure); Serial.print("; ");
  Serial.print(altitude); Serial.print("; ");
  Serial.print(Tc); Serial.print("; ");
  
  Serial.print(ax); Serial.print("; ");
  Serial.print(ay); Serial.print("; ");
  Serial.print(az); Serial.print("; ");
  
  Serial.print(tempBME); Serial.print("; ");
  Serial.print(pressBME); Serial.print("; ");
  Serial.print(altBME); Serial.print("; ");
  Serial.print(humBME); Serial.print("; ");

  Serial.println();  
  
}

void logValues() {
  logFile = SD.open(fileName, FILE_WRITE); // opens the named file, creates new file if it doesnt exist

  logFile.print(millis()); logFile.print("; ");

  logFile.print(pressure); logFile.print("; ");
  logFile.print(altitude); logFile.print("; ");
  logFile.print(Tc); logFile.print("; ");
  
  logFile.print(ax); logFile.print("; ");
  logFile.print(ay); logFile.print("; ");
  logFile.print(az); logFile.print("; ");
  
  logFile.print(tempBME); logFile.print("; ");
  logFile.print(pressBME); logFile.print("; ");
  logFile.print(altBME); logFile.print("; ");
  logFile.print(humBME); logFile.print("; ");

  logFile.println();
  logFile.close();  
}
