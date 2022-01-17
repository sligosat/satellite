
// Accelerometer readings from Adafruit LIS3DH via I2C

#include <Wire.h> //I2C Library

// Must install both libraries below before compiling
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

// Creates I2C object
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

void setup(void) {
  Serial.begin(9600);

  if (! lis.begin(0x18)) { // Stops program while trying to open connection with accelerometer over I2C
    Serial.println("Failed to connect to LIS3DH accelerometer!");
    while (1) yield();
  }
  Serial.println("LIS3DH found!");
  lis.setRange(LIS3DH_RANGE_4_G);   // You can set the max acceleration range here - 2, 4, 8 or 16 G
  lis.setDataRate(LIS3DH_DATARATE_10_HZ); // You can set the datarate to 1, 10, 25, 50, 100, 200 or 400 Hz
  
  Serial.print("Range = "); Serial.print(lis.getRange()); Serial.print("G   "); // Prints the acceleration range
  Serial.print("Data rate = "); Serial.print(lis.getDataRate()); Serial.println("Hz"); // Prints the datarate
  
}

void loop() {
  printLIS3DHvalues();
  delay(500);
}

void printLIS3DHvalues () {
  lis.read(); // Read the raw sensor data
  // You can access the raw data as lis.x, lis.y, lis.z

  sensors_event_t event; // Creates a "normalised" sensor event
  lis.getEvent(&event);

  // Print results from the acceleration event, measured in m/s^2)
  Serial.print("X: "); Serial.print(event.acceleration.x);
  Serial.print(" Y: "); Serial.print(event.acceleration.y);
  Serial.print(" Z: "); Serial.print(event.acceleration.z);
  Serial.println(" m/s^2 ");
}
