// I2C connection of BMP280 to measure altitude and temp
// connected to Nano using I2C Protocol
// 5v
// A5 SCL
// A4 SDA

#include  <Adafruit_BMP280.h>
#define BMP280_ADDRESS 0x76
Adafruit_BMP280 bmp;

float initPressure;

void setup()  {
  Serial.begin(9600);
  Serial.println("Begin");

  if  (!bmp.begin(BMP280_ADDRESS)) {
    Serial.println("Could not connect to BMP280");
    while (1);
  }

  /* Default settings from datasheet.  */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500);  /* Standby time. */
  
  Serial.println("Calibrate BMP280");

  initPressure = bmp.readPressure()/100;
}

void loop() {
    Serial.print("Temperature  = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure()/100);
    Serial.println("  hPa");

    Serial.print("Approx altitude = ");
    Serial.print(bmp.readAltitude(initPressure));
    Serial.println("  m");

    Serial.println();
    delay(5000);
}
