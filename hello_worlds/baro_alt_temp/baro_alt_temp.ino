#include <Adafruit_MPL3115A2.h>

Adafruit_MPL3115A2 baro;

void setup() {
  Serial.begin(9600);
  while(!Serial);

  // init baro
  Serial.println("Init Baro");
  if (!baro.begin()) {
    Serial.println("Baro Failure");
    while(1);
  }
  // set init altitude
  delay(10000);
  baro.setSeaPressure(baro.getPressure());
  delay(2000);

  Serial.println("Ready");
}

void loop() {

  float pressure = baro.getPressure();
  float altitude_ft = baro.getAltitude() * 3.28084;
  float temperature_f = baro.getTemperature() * 9.0000 / 5.0000 + 32.0000;

  Serial.print(pressure);
  Serial.print(",");
  Serial.print(altitude_ft);
  Serial.print(",");  
  Serial.println(temperature_f);  

  delay(2000);
}