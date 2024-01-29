// Connect to SD reader
// 5v
// D11 MOSO
// D12 MISO
// D13 SCK
// D7 CS (pick whatever)

#include <SD.h>

const int CS_PIN = 7;

void setup() {
  Serial.begin(9600);
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

  File datafile = SD.open("data.csv", FILE_WRITE);
  datafile.print(timestamp);
  datafile.print(",");
  datafile.println("Hello World");
  datafile.close();

  delay(1000);

}
