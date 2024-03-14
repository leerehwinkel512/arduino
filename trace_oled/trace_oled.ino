#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int X_pin = 6; // analog pin connected to X output
const int Y_pin = 7; // analog pin connected to Y output

void setup() {
  Serial.begin(9600);

  Serial.println("begin!");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.display();
}


// starting position in maze
int x_val = 16;
int y_val = 16;
int x_val_new = x_val;
int y_val_new = y_val;


void matrixSetLed(int x, int y, bool b){
  if(b){
    display.drawPixel(x, y, SSD1306_WHITE);
  }
  else {
    display.drawPixel(x, y, SSD1306_BLACK);
  }
}

int checkWalls(int x, int y) {

  // outside bounary check
  if (x < 0) {
    return 0;
  }
  if (x > 128-1) {
    return 0;
  }
  if (y < 0) {
    return 0;
  }
  if (y > 64-1) {
    return 0;
  }

  // check maze wall
//  if (mazeArray[x][y] == 1){
//    Serial.println("wall: Maze");
//    return 0;
//  }

  return 1;
}

void loop() {

  //delay(20);

  x_val_new = x_val;
  y_val_new = y_val;

  if (analogRead(Y_pin) < 400) {
    x_val_new = x_val_new + 1;
    if (checkWalls(x_val_new, y_val_new) == 1){
      //matrixSetLed(x_val,y_val,false);
      x_val = x_val + 1;
      x_val_new = x_val;
      y_val_new = y_val;
      matrixSetLed(x_val,y_val,true);
      display.display();
    }
  }

  if (analogRead(Y_pin) > 700) {
    x_val_new = x_val_new - 1;
    if (checkWalls(x_val_new, y_val_new) == 1){
      //matrixSetLed(x_val,y_val,false);
      x_val = x_val - 1;
      x_val_new = x_val;
      y_val_new = y_val;
      matrixSetLed(x_val,y_val,true);
      display.display();      
    }
  }

  if (analogRead(X_pin) < 400) {
    y_val_new = y_val_new - 1;
    if (checkWalls(x_val_new, y_val_new) == 1){
      //matrixSetLed(x_val,y_val,false);
      y_val = y_val - 1;
      x_val_new = x_val;
      y_val_new = y_val;
      matrixSetLed(x_val,y_val,true);
      display.display();      
    }
  }
  
  if (analogRead(X_pin) > 700) {
    y_val_new = y_val_new + 1;
    if (checkWalls(x_val_new, y_val_new) == 1){
      //matrixSetLed(x_val,y_val,false);
      y_val = y_val + 1;
      x_val_new = x_val;
      y_val_new = y_val;
      matrixSetLed(x_val,y_val,true);
      display.display();      
    }
  }
}
