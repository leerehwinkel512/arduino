#include "WiFi.h"
#include <esp_now.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// Joystick Analog
const int X_pin = A6;
const int Y_pin = A7;
int x_pos;
int y_pos;

const int maxspeed = 200;

// Define a message structure
typedef struct struct_message {
  int speed;
  int direction;
} struct_message;
struct_message myData;

// MAC Address of receiver
uint8_t receiverAddress[] = {0xEC, 0xDA, 0x3B, 0x61, 0x92, 0xEC};

// Peer info
esp_now_peer_info_t peerInfo;

// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {

  // display data
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Sending:");
  display.print("Speed: ");
  display.println(myData.speed);
  display.print("Direction: ");
  display.println(myData.direction);
  display.println();
  display.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
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

  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  
  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    display.println("Failed to add peer");
    display.display();
    return;
  }

  // init joystick
  pinMode(X_pin, INPUT);
  pinMode(Y_pin, INPUT);

}

void loop() {

  x_pos = analogRead(X_pin);
  y_pos = analogRead(Y_pin);

  // Motor
  if (x_pos < 1000){
    myData.speed = map(x_pos, 1000, 0, 0, maxspeed);
  }
  else if (x_pos > 3000){
    myData.speed = -1 * map(x_pos, 3000, 4095, 0, maxspeed);
  }
  else { // A off
    myData.speed = 0; 
  }

  // Direction
  if (y_pos < 500){
    myData.direction = -1;
  }
  else if (y_pos > 3500){
    myData.direction = 1;
  }
  else { // A off
    myData.direction = 0;
  }   

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(receiverAddress, (uint8_t *) &myData, sizeof(myData));

  delay(200);

}
