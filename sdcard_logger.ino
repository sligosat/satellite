/*
  SD card - log data to text file
  For use with sparkfun microSD breakout with level shifting
 * SD card module attached to SPI bus as follows:
 *
 ** MOSI  - OI - pin 11
 ** MISO  - OO - pin 12
 ** CLK   - SCK - pin 13
 ** CS    - CS -  pin 4 
 * 
 * Pin 10 on Arduino Uno must be kept <<unused>>, in order for SD card library to work properly!!
 */

#include <SPI.h>
#include <SD.h>

const int SDSelectPin = 4; // Set up your CS pin here

File logFile; // Sets up the SD card object
char fileName[] = "log2.txt"; // Define a filename here, max 8 characters long

void setup() {
  Serial.begin(9600);
  Serial.print("Initializing SD card...");

  if (!SD.begin(SDSelectPin)) { // Trys to open the SD card connection on the CS pin
    Serial.println("SD card initialization failed!");
    while (1) yield();
  }
  Serial.println("SD card initialization done.");
  logFile = SD.open(fileName, FILE_WRITE);
  logFile.println("start testing 1, 2, 3.");
  logFile.close();
}

void loop() {
  logFile = SD.open(fileName, FILE_WRITE); // opens the named file, creates new file if it doesnt exist
  if (logFile) { // Checks if the file is ready to be used
    writeData(); // Writes in the data
  } else {
    Serial.println("SD card error, no file to write to!");  
  }
  
  logFile.close(); // Remember to close the file when done!
  delay(1000);
  
}

void writeData() {
  logFile.println("hello world"); // Prints this line to the open file on sd card
  Serial.println("wrote hello world to file");
  }
