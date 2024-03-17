#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Stepper.h>

const int stepsPerRev = 2038;
#define step_in1 3
#define step_in2 4
#define step_in3 5
#define step_in4 6

Stepper stepper(stepsPerRev, step_in1, step_in3, step_in2, step_in4);

int direction  = 0;

const int turn_size = 50;
const int turn_limit = 5;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
char buf[25]; 


// Joystick Analog
const int X_pin = A6;
const int Y_pin = A7;
int x_pos;
int y_pos;

// Motor A connections
int EN_A = 10;
int IN1 = 11;
int IN2 = 12;

// Motor B connections
int IN3 = 7;
int IN4 = 8;
int EN_B = 9;

int motor_speed_A;
int motor_speed_B;

const int maxspeed_A = 100;
const int maxspeed_B = 100;


void setup() {
//  Serial.begin(9600);
//  Serial.println("begin!");

  // init display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
//    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();  // Adafruit logo
  delay(2000);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

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

  // init joystick
  pinMode(X_pin, INPUT);
  pinMode(Y_pin, INPUT);

  // init stepper
  stepper.setSpeed(10);
}

void loop() {

  x_pos = analogRead(X_pin);
  y_pos = analogRead(Y_pin);

  // display position
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Joystick"));
  sprintf(buf, "X: %d", x_pos);
  display.println(buf);  
  sprintf(buf, "Y: %d", y_pos);
  display.println(buf);
  display.println();
  display.println(F("Motors"));  


  // Motor A
  if (x_pos < 400){
    motor_speed_A = map(x_pos, 400, 0, 0, maxspeed_A);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(EN_A, motor_speed_A);
    sprintf(buf, "A Speed: + %d", motor_speed_A);
    display.println(buf);
  }
  else if (x_pos > 600){ // A Counter Clockwise
    motor_speed_A = map(x_pos, 600, 1023, 0, maxspeed_A);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(EN_A, motor_speed_A);
    sprintf(buf, "A Speed: - %d", motor_speed_A);
    display.println(buf);    
  }
  else { // A off
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    sprintf(buf, "A Speed: off");
    display.println(buf);   
  }    

  // Motor B
  if (x_pos < 400){ // B Clockwise
    motor_speed_B = map(x_pos, 400, 0, 0, maxspeed_B);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(EN_B, motor_speed_B);
    sprintf(buf, "B Speed: + %d", motor_speed_B);
    display.println(buf);    
  }
  else if (x_pos > 600){ // B Counter Clockwise
    motor_speed_B = map(x_pos, 600, 1023, 0, maxspeed_B);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(EN_B, motor_speed_B);
    sprintf(buf, "B Speed: - %d", motor_speed_B);
    display.println(buf);       
  }
  else { // B off
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    sprintf(buf, "B Speed: off");
    display.println(buf);    
  }

  // Stepper
  if (y_pos > 800){ // Turn Left
    if (direction > -turn_limit){
      stepper.step(-turn_size);
      direction = direction - 1;
      display.println(F("Stepper: Left"));      
    }
    else{
      display.println(F("Stepper: Limit"));
    }
  }
  else if (y_pos < 200){ // Turn Right
    if (direction < turn_limit){
      stepper.step(turn_size);
      direction = direction + 1;
      display.println(F("Stepper: Right"));      
    }
    else{
      display.println(F("Stepper: Limit"));
    }    
  }
  else { // center turn
    if (direction < 0){
      stepper.step(turn_size);
      direction = direction + 1;
      display.println(F("Stepper: Recenter"));
    }
    else if (direction > 0){
      stepper.step(-turn_size);
      direction = direction - 1;
      display.println(F("Stepper: Recenter"));
    }
    else {
      display.println(F("Stepper: Center"));
    }
  }

  display.display();
}
