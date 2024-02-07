#include "LedControl.h"

//data, clock, chip select
LedControl lc1=LedControl(11,13,12,8);
LedControl lc2=LedControl(5,7,6,8);


typedef struct {
    int g;
    int m;
    int x;
    int y;    
} LEDTuple;


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

void loop() {

  LEDTuple w;

  for(int x=0; x<32; x++) {
    for(int y=0; y<32; y++) {
      w = getMatrixAddress(x, y);

      if(w.g == 1){
        lc1.setLed(w.m, w.x, w.y, true);
        delay(50);
        lc1.setLed(w.m, w.x, w.y, false);
      }
      if(w.g == 2){
        lc2.setLed(w.m, w.x, w.y, true);
        delay(50);
        lc2.setLed(w.m, w.x, w.y, false);
      }
    }
  }
}
