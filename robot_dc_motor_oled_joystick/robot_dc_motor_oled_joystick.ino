#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
char buf[200]; 


// Joystick Analog
const int X_pin = 6;
const int Y_pin = 7;
int x_pos;
int y_pos;

// Motor A connections
int EN_A = 10;
int IN1 = 9;
int IN2 = 8;

// Motor B connections
int IN3 = 7;
int IN4 = 6;
int EN_B = 5;

int motor_speed_A;
int motor_speed_B;

const int maxspeed_A = 100;
const int maxspeed_B = 75;


void setup() {
  Serial.begin(9600);

  Serial.println("begin!");

  // init display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while(1);
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
}

void loop() {

  x_pos = analogRead(X_pin);
  y_pos = analogRead(Y_pin);

  // display position
  display.clearDisplay();
  display.setCursor(0, 0);
  sprintf(buf, "X: %d", x_pos);
  display.println(buf);  
  sprintf(buf, "Y: %d", y_pos);
  display.println(buf);
  display.println();


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

  display.display();
}
