#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

void matrixSetLed(int x, int y, bool b){
  if(b){
    display.drawPixel(x, y, SSD1306_WHITE);
  }
  else {
    display.drawPixel(x, y, SSD1306_BLACK);
  }
}

int checkWall_x(int x) {

  if (x < 0) {
    return 0;
  }
  if (x > 128-1) {
    return 0;
  }

  return 1;
}

int checkWall_y(int y) {

  if (y < 0) {
    return 0;
  }
  if (y > 64-1) {
    return 0;
  }

  return 1;
}

void setup() {

  // init display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for(;;);
  }
  display.display();  // Adafruit logo
  delay(2000);
  display.clearDisplay();
  display.display();

	// init MPU
	if (!mpu.begin()) {
    
		for(;;);
	}

	mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
	mpu.setGyroRange(MPU6050_RANGE_250_DEG);
	mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

	delay(100);

}


// starting position in maze
int x_pos = 64;
int y_pos = 32;
int x_pos_new;
int y_pos_new;

const float grav = 0.2;
const float friction = 0.02;

float velocity_x = 0;
float velocity_y = 0;
float accel_x = 0;
float accel_y = 0;

sensors_event_t a, g, temp;
float AcX;
float AcY;
float AcZ;
float x_rad;
float y_rad;
float x_deg;
float y_deg;

void loop() {

	mpu.getEvent(&a, &g, &temp);

  // calc tilt angle
  AcX = a.acceleration.x;
  AcY = a.acceleration.y;
  AcZ = a.acceleration.z;

  x_rad = atan2(AcY, sqrt(AcX * AcX + AcZ * AcZ));
  y_rad = atan2(AcX, sqrt(AcY * AcY + AcZ * AcZ));

  x_deg = RAD_TO_DEG * x_rad;
  y_deg = RAD_TO_DEG * y_rad;


  // calc acceleration based on tilt angle
  if (x_deg > 0) {
    accel_x = grav * cos(abs(x_rad));
  }
  else if (x_deg < 0) {
    accel_x = -1.0 * grav * cos(abs(x_rad));
  }
  else {
    accel_x = 0;
  }

  if (y_deg > 0) {
    accel_y = grav * cos(abs(y_rad));
  }
  else if (y_deg < 0) {
    accel_y = -1.0 * grav * cos(abs(y_rad));
  }
  else {
    accel_y = 0;
  }  


  // calc velocity
  velocity_x = velocity_x + accel_x - velocity_x * friction;
  velocity_y = velocity_y + accel_y - velocity_y * friction;

  // calc new position
  x_pos_new = x_pos + int(velocity_x);
  y_pos_new = y_pos + int(velocity_y);

  matrixSetLed(x_pos,y_pos,false);

  if (checkWall_x(x_pos_new)){
    x_pos = x_pos_new;
  }
  else {
    velocity_x = 0;
  }

  if (checkWall_y(y_pos_new)){
    y_pos = y_pos_new;
  }  
  else {
    velocity_y = 0;
  }

  matrixSetLed(x_pos,y_pos,true);
  display.display();

}
