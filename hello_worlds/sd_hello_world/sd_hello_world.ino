// Connect to SD reader
// D11 MOSI
// D12 MISO
// D13 SCK
// D7 CS

#include <SD.h>

const int CS_PIN = 7;

void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.println("Init SD");

  pinMode(CS_PIN, OUTPUT);
  
  if(!SD.begin(CS_PIN))
  {
    Serial.println("SD Failure");
    while(1);
  }

  Serial.println("SD Ready");
}

void loop() {

  long timestamp = millis();

  File dataFile = SD.open("data.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.print(timestamp);
    dataFile.print(",");
    dataFile.println("Hello World");
    dataFile.close();
  } else {
    Serial.println("Error opening data.csv");
  }

  delay(1000);

}
