#include <esp_now.h>
#include <WiFi.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define step_in1 3
#define step_in2 4
#define step_in3 5
#define step_in4 6

int steps[4][4] = {
  {HIGH, HIGH, LOW,  LOW},
  {LOW,  HIGH, HIGH, LOW},
  {LOW,  LOW,  HIGH, HIGH},
  {HIGH, LOW,  LOW,  HIGH}
};

unsigned long lastTransfer = 0;
unsigned long currentTime;
unsigned long timeSinceLastTransfer;
const int waitTimeLimit = 1000;

int direction  = 0;
const int step_delay = 10;
const int turn_size = 5;
const int turn_limit = 10;


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// Motor A connections
int EN_A = 10;
int IN1 = 11;
int IN2 = 12;

// Motor B connections
int IN3 = 7;
int IN4 = 8;
int EN_B = 9;

// Define a message structure
typedef struct struct_message {
  int speed;
  int direction;
} struct_message;
struct_message myData;


// stepper driver function
void turnstepper(int step_num, int step_delay, int direction){

  if (direction == 1) {
    for(int j = 1; j<=step_num; j++){
      for(int i = 0; i < 4; i++) {
        digitalWrite(step_in1, steps[i][0]);
        digitalWrite(step_in2, steps[i][1]);
        digitalWrite(step_in3, steps[i][2]);
        digitalWrite(step_in4, steps[i][3]);
        delay(step_delay);
      }
    }
  }
  else if (direction == -1) {
    for(int j = 1; j<=step_num; j++){
      for(int i = 3; i >= 0; i--) {
        digitalWrite(step_in1, steps[i][0]);
        digitalWrite(step_in2, steps[i][1]);
        digitalWrite(step_in3, steps[i][2]);
        digitalWrite(step_in4, steps[i][3]);
        delay(step_delay);
      }
    }
  }

}


// Callback function executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {

  // update kill time
  lastTransfer = millis();

  // copy message data to struct
  memcpy(&myData, incomingData, sizeof(myData));


  // display data
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Received:");
  display.print("Speed: ");
  display.println(myData.speed);
  display.print("Direction: ");
  display.println(myData.direction);
  display.println();


  // motor
  if (myData.speed > 0){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);    
    analogWrite(EN_A, myData.speed);
    analogWrite(EN_B, myData.speed);    
  }
  else if (myData.speed < 0){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);    
    analogWrite(EN_A, myData.speed);
    analogWrite(EN_B, myData.speed);    
  }
  else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }


  // direction
  if (myData.direction > 0){ // Turn Left
    if (direction > -turn_limit){
      turnstepper(turn_size, step_delay, -1);
      direction = direction - 1;
      display.println("Stepper: Left");      
    }
    else{
      display.println("Stepper: Limit");
    }
  }
  else if (myData.direction < 0){ // Turn Right
    if (direction < turn_limit){
      turnstepper(turn_size, step_delay, 1);
      direction = direction + 1;
      display.println("Stepper: Right");      
    }
    else{
      display.println("Stepper: Limit");
    }    
  }
  else { // center turn
    if (direction < 0){
      turnstepper(turn_size, step_delay, 1);
      direction = direction + 1;
      display.println("Stepper: Recenter");
    }
    else if (direction > 0){
      turnstepper(turn_size, step_delay, -1);
      direction = direction - 1;
      display.println("Stepper: Recenter");
    }
    else {
      display.println("Stepper: Center");
    }
  }

  display.display();
}


void setup() {

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // init display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for(;;);
  }
  display.display();  // Adafruit logo
  delay(2000);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // show MAC
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("MAC Address:");
  display.println(WiFi.macAddress());
  display.display();
  delay(2000);
 
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    display.println("Error initializing ESP-NOW");
    display.display();
    return;
  }
  
  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);

  // Ready for data
  display.println();
  display.println("Waiting for data...");
  display.display();  

	// Set all the motor control pins to outputs
	pinMode(EN_A, OUTPUT);
	pinMode(EN_B, OUTPUT);
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(IN1, LOW);
	digitalWrite(IN2, LOW);
	digitalWrite(IN3, LOW);
	digitalWrite(IN4, LOW);

  // init stepper
  pinMode(step_in1, OUTPUT);
  pinMode(step_in2, OUTPUT);
  pinMode(step_in3, OUTPUT);
  pinMode(step_in4, OUTPUT);
}

void loop() {

  // check kill
  currentTime = millis();
  timeSinceLastTransfer = currentTime - lastTransfer;
  if (timeSinceLastTransfer >= waitTimeLimit){

    // Turn off motors
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    // display data
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("No Signal");
    display.println("Kill Motor");
    display.display();

  }
}
