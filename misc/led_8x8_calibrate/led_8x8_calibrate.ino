#include "LedControl.h"

//data, clock, chip select
LedControl lc1=LedControl(11,13,12,8);
LedControl lc2=LedControl(5,7,6,8);

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

  // x,y

  for(int index=0;index<lc1.getDeviceCount();index++) {
    lc1.setLed(index,1,1,true);
    lc1.setLed(index,1,2,true);
    lc1.setLed(index,2,1,true);
    lc1.setLed(index,3,1,true);
  }

  for(int index=0;index<lc2.getDeviceCount();index++) {
    lc2.setLed(index,1,1,true);
    lc2.setLed(index,1,2,true);
    lc2.setLed(index,2,1,true);
    lc2.setLed(index,3,1,true);
  }

}
