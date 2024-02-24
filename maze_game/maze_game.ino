#include "LedControl.h"

//data, clock, chip select
LedControl lc1=LedControl(10,12,11,8);
LedControl lc2=LedControl(9,7,8,8);


typedef struct {
    int g;
    int m;
    int x;
    int y;    
} LEDTuple;

const int X_pin = 6; // analog pin connected to X output
const int Y_pin = 7; // analog pin connected to Y output


LEDTuple getMatrixAddress(int x, int y) {

  LEDTuple q; 

  if (x>=0 && x<8){
    if(y>=0 && y<8){
      q.g=1;
      q.m=0;
      q.x=x;
      q.y=8-1-y;
    }
    if(y>=8 && y<16){
      q.g=1;
      q.m=1;
      q.x=x;
      q.y=16-1-y;
    }
    if(y>=16 && y<24){
      q.g=1;
      q.m=2;
      q.x=x;
      q.y=24-1-y;
    }
    if(y>=24 && y<32){
      q.g=1;
      q.m=3;
      q.x=x;
      q.y=32-1-y; 
    }
  }

  if (x>=8 && x<16){
    if(y>=0 && y<8){
      q.g=1;
      q.m=7;
      q.x=16-1-x;
      q.y=y;
    }
    if(y>=8 && y<16){
      q.g=1;
      q.m=6;
      q.x=16-1-x;
      q.y=y-8;
    }
    if(y>=16 && y<24){
      q.g=1;
      q.m=5;
      q.x=16-1-x;
      q.y=y-16;
    }
    if(y>=24 && y<32){
      q.g=1;
      q.m=4;
      q.x=16-1-x;
      q.y=y-24;
    }
  }

  if (x>=16 && x<24){
    if(y>=0 && y<8){
      q.g=2;
      q.m=0;
      q.x=x-16;
      q.y=8-1-y;
    }
    if(y>=8 && y<16){
      q.g=2;
      q.m=1;
      q.x=x-16;
      q.y=16-1-y;
    }
    if(y>=16 && y<24){
      q.g=2;
      q.m=2;
      q.x=x-16;
      q.y=24-1-y;
    }
    if(y>=24 && y<32){
      q.g=2;
      q.m=3;
      q.x=x-16;
      q.y=32-1-y;
    }
  }  

  if (x>=24 && x<32){
    if(y>=0 && y<8){
      q.g=2;
      q.m=7;
      q.x=32-1-x;
      q.y=y;
    }
    if(y>=8 && y<16){
      q.g=2;
      q.m=6;
      q.x=32-1-x;
      q.y=y-8;
    }
    if(y>=16 && y<24){
      q.g=2;
      q.m=5;
      q.x=32-1-x;
      q.y=y-16;
    }
    if(y>=24 && y<32){
      q.g=2;
      q.m=4;
      q.x=32-1-x;
      q.y=y-24;
    }
  }

  return q;
}

void matrixSetLed(int x, int y, bool b){
  LEDTuple w;
  w = getMatrixAddress(x, y);

  if(w.g == 1){
    lc1.setLed(w.m, w.x, w.y, b);
  }
  if(w.g == 2){
    lc2.setLed(w.m, w.x, w.y, b);
  }
}

int checkWalls(int x, int y) {

  // outside bounary check
  if (x < 0) {
    return 0;
  }
  if (x > 31) {
    return 0;
  }
  if (y < 0) {
    return 0;
  }
  if (y > 31) {
    return 0;
  }

  // check maze wall
//  if (mazeArray[x][y] == 1){
//    Serial.println("wall: Maze");
//    return 0;
//  }

  return 1;
}

void setup() {

  for(int index=0;index<lc1.getDeviceCount();index++) {
    lc1.shutdown(index,false);
    lc1.clearDisplay(index);
    lc1.setIntensity(index,1);
  }  

  for(int index=0;index<lc2.getDeviceCount();index++) {
    lc2.shutdown(index,false);
    lc2.clearDisplay(index);
    lc2.setIntensity(index,1);
  }  

}

// starting position in maze
int x_val = 16;
int y_val = 16;
int x_val_new = x_val;
int y_val_new = y_val;


void loop() {

  matrixSetLed(x_val,y_val,true);
  delay(100);

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
    }
  }
}
