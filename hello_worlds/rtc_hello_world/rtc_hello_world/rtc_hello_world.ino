// RTC DS3231 connected to Nano using I2C Protocol
// 5v
// A5 SCL
// A4 SDA

#include <Wire.h>
#include <I2C_RTC.h>

static DS3231 RTC;

void setup() {
  Serial.begin(9600);
  RTC.begin();

  if (RTC.isRunning())
    Serial.println("RTC running");
  else
    Serial.println("RTC NOT running");

// uncomment to set RTC
/*
  Serial.println("Setting Time");
  RTC.setHourMode(CLOCK_H12);
  RTC.setDateTime(__DATE__, __TIME__);
  RTC.updateWeek();
  Serial.println("New Time Set");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);
  RTC.startClock(); 
*/
}

void loop() {
  Serial.print(" ");
  Serial.print(RTC.getDay());
  Serial.print("-");
  Serial.print(RTC.getMonth());
  Serial.print("-");
  Serial.print(RTC.getYear());

  Serial.print(" ");

  Serial.print(RTC.getHours());
  Serial.print(":");
  Serial.print(RTC.getMinutes());
  Serial.print(":");
  Serial.print(RTC.getSeconds());
  if (RTC.getHourMode() == CLOCK_H12)
  {
    switch (RTC.getMeridiem())
    {
      case HOUR_AM :
        Serial.print(" AM");
        break;
      case HOUR_PM :
        Serial.print(" PM");
        break;
    }
  }
  Serial.println();
  delay(1000);
}
