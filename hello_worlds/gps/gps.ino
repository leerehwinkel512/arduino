#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <SD.h>

#define SD_CS_PIN 5

#include <TinyGPSPlus.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define RXPin 3
#define TXPin 2

TinyGPSPlus gps;

void setup() {

  // init display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for(;;);
  }
  display.display();  // Adafruit logo
  delay(2000);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // init gps serial
  Serial1.begin(9600, SERIAL_8N1, RXPin, TXPin);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Searching... ");
  display.display();

  // init SD
  pinMode(SD_CS_PIN, OUTPUT);
  
  if(!SD.begin(SD_CS_PIN))
  {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("SD Failure");
    display.display();

    for(;;);
  }

  File datafile = SD.open("data.csv", FILE_WRITE);
  datafile.println("date,time,lat_dec,lng_dec,speed_mph,alt_ft");
  datafile.close();

  delay(1000);  

}

void loop() {

  while (Serial1.available() > 0){
    gps.encode(Serial1.read());
    if (gps.location.isUpdated()){

      // record data
      File datafile = SD.open("data.csv", FILE_WRITE);

      datafile.print(gps.date.month()); 
      datafile.print("-");
      datafile.print(gps.date.day());
      datafile.print(",");
      datafile.print(gps.time.hour()); 
      datafile.print(":");
      datafile.print(gps.time.minute()); 
      datafile.print(":");
      datafile.print(gps.time.second());
      datafile.print(",");
      datafile.print(gps.location.lat());
      datafile.print(",");   
      datafile.print(gps.location.lng());
      datafile.print(",");
      datafile.print(gps.speed.mph());
      datafile.print(",");
      datafile.print(gps.altitude.feet());
      datafile.println();
      datafile.close();
      
      // display
      display.clearDisplay();
      display.setCursor(0, 0);

      display.print("Satellites: "); 
      display.print(gps.satellites.value());
      display.println();

      display.print("Lat: "); 
      display.print(convertLatLng(gps.location.lat(), false));
      display.println();         
      display.print("Lng: "); 
      display.print(convertLatLng(gps.location.lng(), true));
      display.println();

      display.print(gps.date.month()); 
      display.print("/");
      display.print(gps.date.day());
      display.print(" ");
      display.print(gps.time.hour()); 
      display.print(":");
      display.print(gps.time.minute()); 
      display.print(":");
      display.print(gps.time.second());
      display.println();

      display.print("Speed (mi/h): ");
      display.print(gps.speed.mph()); 
      display.println();

      display.print("Course (deg): "); 
      display.print(gps.course.deg());
      display.println();

      display.print("Alt (ft): "); 
      display.print(gps.altitude.feet());
      display.println();

      display.display();
    }
  }

  delay(5000);

}


String convertLatLng(double decimalDegrees, bool islng) {

  char sign;

  if (islng) {
    sign = (decimalDegrees < 0) ? 'W' : 'E';
  }
  else {
    sign = (decimalDegrees < 0) ? 'S' : 'N';
  }

  decimalDegrees = abs(decimalDegrees);
  long degrees = long(decimalDegrees);
  double decimalMinutes = (decimalDegrees - degrees) * 60;
  int minutes = int(decimalMinutes);
  double seconds = (decimalMinutes - minutes) * 60;

  String deg = String(degrees) + " " + String(minutes) + "' " + String(seconds, 3) + "\"" + String(sign);

  return deg;
}