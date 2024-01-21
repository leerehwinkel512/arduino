// temp sensor
#include "DHT.h"

#define DHTPIN 3
#define DHTTYPE DHT22

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

DHT dht(DHTPIN, DHTTYPE);


// scroll
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

#define MAX_DEVICES 4
#define CLK_PIN     13
#define DATA_PIN    9
#define CS_PIN      11

MD_Parola P = MD_Parola(HARDWARE_TYPE,DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

#define	BUF_SIZE 200
char curMessage[BUF_SIZE] = { "Start" };
char newMessage[BUF_SIZE] = { "New" };
bool newMessageAvailable = false;
bool showTemp = true;


void setup() {
  Serial.begin(9600);

  dht.begin();

  P.begin();
  P.displayText(curMessage, PA_CENTER, 50, 9000, PA_SCROLL_UP, PA_SCROLL_UP);
}


// time between updates
unsigned long previousMillis = 0;
const long interval = 10000;  

void loop() {

  unsigned long currentMillis = millis();

  // update reading
  if (currentMillis - previousMillis >= interval){

    previousMillis = currentMillis;  

    float h = dht.readHumidity();
    float f = dht.readTemperature(true);

    // Check if any reads failed
    if (isnan(h) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
    }
    Serial.println();
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(f);
    Serial.println(F("°F "));


    // update message, switch between temp and humidity
    if (showTemp){
      String f_str = String(f, 1) + "\xB0"; 
      strcpy(newMessage, f_str.c_str());
      showTemp = false;
    }
    else{
      String h_str = String(h, 1) + "%"; 
      strcpy(newMessage, h_str.c_str());
      showTemp = true;      
    }

    newMessageAvailable = true;
  }

  P.displayAnimate();

  if (newMessageAvailable) {
    Serial.println("Update message.");

    P.displayClear();
    strcpy(curMessage, newMessage);
    P.displayReset();

    newMessageAvailable = false;
  }    
}