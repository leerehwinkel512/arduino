/*
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/10498514-4be2-4603-a728-e920f69c885e

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  String text_scroll;
  float temperature;
  bool led;
  bool yellow_button;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 10
#define DHTTYPE DHT22

const int BUTTON_PIN = 7;
const int LED_PIN = 4;

DHT dht(DHTPIN,DHTTYPE);

void setup() {
  Serial.begin(9600);
  delay(1500);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  dht.begin();
}

void loop() {
  ArduinoCloud.update();

  // on button press
  int ledState = digitalRead(LED_PIN);
  int buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == LOW){
    
    if (ledState){
      digitalWrite(LED_PIN, LOW);
      led = false;
      delay( 100 );
    }else{
      digitalWrite(LED_PIN, HIGH);
      led = true;
      delay( 100 );
    }

    temperature = dht.readTemperature(true);
    
  }
  delay( 100 );
}


/*
  Since Led is READ_WRITE variable, onLedChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onLedChange()  {

  if (led) {
    digitalWrite(LED_PIN, HIGH);
    text_scroll = "Turn On";
  } else {
    digitalWrite(LED_PIN, LOW);
    text_scroll = "Turn Off";
  }
}

/*
  Since TextScroll is READ_WRITE variable, onTextScrollChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onTextScrollChange()  {

  if (text_scroll == "OFF") {
    digitalWrite(LED_PIN, LOW);
    led = false;
  }

  if (text_scroll == "ON") {
    digitalWrite(LED_PIN, HIGH);
    led = true;
  }

}

/*
  Since YellowButton is READ_WRITE variable, onYellowButtonChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onYellowButtonChange()  {

  if (yellow_button) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

}