#include "SdFat.h"

// SD card chip select pin
const uint8_t SD_CS_PIN = 7;

// File system object.
SdFat sd;

// Create a file object.
SdFile file;

void setup() {
  Serial.begin(115200);
  // Wait for USB Serial 
  while (!Serial) {
    delay(100);
  }

  // Initialize the SD card at SPI_HALF_SPEED to ensure better reliability at different card's speed grades.
  if (!sd.begin(SD_CS_PIN, SPI_HALF_SPEED)) {
    Serial.println("SD Card initialization failed!");
    return;
  }
  Serial.println("SD Card initialized.");

  // Create a new file and open it for writing
  if (!file.open("hello.txt", O_RDWR | O_CREAT | O_TRUNC)) {
    Serial.println("Opening file failed!");
    return;
  }
  Serial.println("File opened.");

  // Write to file
  file.println("Hello, World!");
  file.close();  // close the file
  Serial.println("Wrote to file 'hello.txt'.");

  // Reopen the file for reading
  if (!file.open("hello.txt", O_READ)) {
    Serial.println("Opening file failed!");
    return;
  }
  Serial.println("Opened file for reading.");

  // Read from the file until there's nothing else in it:
  int data;
  while ((data = file.read()) >= 0) {
    Serial.write(data);
  }
  file.close();  // Close the file
  Serial.println("\nDone reading file.");
}

void loop() {
  // not used in this example
}
