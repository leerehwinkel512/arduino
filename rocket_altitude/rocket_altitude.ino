// Connect to SD reader (SPI)
// 5v
// D11 MOSO
// D12 MISO
// D13 SCK
// D7 CS

// Connect to Baro reader (I2C)
// 3.3v
// A4 SDA
// A5 SCL

#include <Adafruit_MPL3115A2.h>
#include <SD.h>

const int CS_PIN = 7;
const int LED_PIN = 5;

Adafruit_MPL3115A2 baro;

void setup() {
  Serial.begin(9600);
  while(!Serial);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.println("Init SD");
  pinMode(CS_PIN, OUTPUT);
  if(!SD.begin(CS_PIN))
  {
    Serial.println("SD Failure");
    while(1);
  }

  // init datafile
  Serial.println("Init Datafile");
  File datafile = SD.open("data.csv", FILE_WRITE);
  datafile.println("-----------------");
  datafile.print("TimeStamp");
  datafile.print(",");
  datafile.print("Pressure_hPa");
  datafile.print(",");
  datafile.print("Altitude_ft");
  datafile.print(",");  
  datafile.println("Temp_F");    
  datafile.close();  


  Serial.println("Init Baro");
  if (!baro.begin()) {
    Serial.println("Baro Failure");
    while(1);
  }
  // set init altitude
  delay(5000);
  baro.setSeaPressure(baro.getPressure());
  delay(2000);

  Serial.println("Ready");
}

void loop() {

  long timestamp = millis();

  float pressure = baro.getPressure();
  float altitude_ft = baro.getAltitude() * 3.28084;
  float temperature_f = baro.getTemperature() * 9.0000 / 5.0000 + 32.0000;

  Serial.print(timestamp);
  Serial.print(",");  
  Serial.print(pressure);
  Serial.print(",");
  Serial.print(altitude_ft);
  Serial.print(",");  
  Serial.println(temperature_f);  

  Serial.println("Save reading");
  File datafile = SD.open("data.csv", FILE_WRITE);
  datafile.print(timestamp);
  datafile.print(",");  
  datafile.print(pressure);
  datafile.print(",");
  datafile.print(altitude_ft);
  datafile.print(",");  
  datafile.println(temperature_f);
  datafile.close();


  digitalWrite(LED_PIN, LOW);
  delay(200);
  digitalWrite(LED_PIN, HIGH);  
}