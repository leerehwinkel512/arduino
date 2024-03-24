#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

#define MAX_DEVICES 8
#define CLK_PIN   13
#define DATA_PIN  9
#define CS_PIN    11

MD_Parola P = MD_Parola(HARDWARE_TYPE,DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
void setup() {
  P.begin();
  P.displayText("Hello World!", PA_CENTER, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void loop(void)
{
  if (P.displayAnimate())
    P.displayReset();
}
