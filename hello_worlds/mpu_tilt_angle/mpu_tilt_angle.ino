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

void setup() {

  // init display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for(;;);
  }
  display.display();  // Adafruit logo
  delay(2000);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

	// init MPU
	if (!mpu.begin()) {
		for(;;);
	}

	mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
	mpu.setGyroRange(MPU6050_RANGE_250_DEG);
	mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

	delay(100);

}

void loop() {

	sensors_event_t a, g, temp;
	mpu.getEvent(&a, &g, &temp);

  display.clearDisplay();
  display.setCursor(0, 0);

  display.println("Accel (m/s^2): X,Y,Z");
	display.print(a.acceleration.x);
	display.print(",");
	display.print(a.acceleration.y);
	display.print(",");
	display.print(a.acceleration.z);

  display.println();
  display.println();  
  display.println("Gyro (deg/s): X,Y,Z");
	display.print(g.gyro.x);
	display.print(",");
	display.print(g.gyro.y);
	display.print(",");
	display.print(g.gyro.z);

  // tilt angle
  float AcX = a.acceleration.x;
  float AcY = a.acceleration.y;
  float AcZ = a.acceleration.z;

  float x_deg = RAD_TO_DEG * atan2(AcY, sqrt(AcX * AcX + AcZ * AcZ));
  float y_deg = RAD_TO_DEG * atan2(AcX, sqrt(AcY * AcY + AcZ * AcZ));

  display.println();
  display.println();  
  display.println("Tilt (deg): X,Y");
	display.print((int)x_deg);
	display.print(",");
	display.print((int)y_deg);  


	display.display();

	delay(100);

}
