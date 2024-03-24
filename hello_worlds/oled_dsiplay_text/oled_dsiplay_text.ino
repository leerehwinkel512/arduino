#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int X_pin = 6; // joystick analog x
const int Y_pin = 7; // joystick analog y

void setup() {
  Serial.begin(9600);

  Serial.println("begin!");

  // init display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while(1);
  }
  display.display();
  delay(1000);

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void displayText(const char* str){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(str);
  display.display();
}

void loop() {
  displayText("hello world");
  delay(2000);

  displayText("next");
  delay(2000);     
}
