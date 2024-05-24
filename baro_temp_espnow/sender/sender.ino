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

#include <Adafruit_MPL3115A2.h>

Adafruit_MPL3115A2 baro;


// Define a message structure
typedef struct struct_message {
  float pressure;
  float altitude_ft;
  float temperature_f;
} struct_message;
struct_message myData;

// MAC Address of receiver
uint8_t receiverAddress[] = {0x08, 0xB6, 0x1F, 0xBA, 0x0D, 0xB4};

// Peer info
esp_now_peer_info_t peerInfo;

// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {

  display.clearDisplay();
  display.setCursor(0, 0);

  display.print("Pressure: ");
  display.println(myData.pressure);

  display.print("Altitude (ft): ");
  display.println(myData.altitude_ft);

  display.print("Temp (f): ");
  display.println(myData.temperature_f);

  display.print("Sending: ");
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

  // init baro
  Serial.println("Init Baro");
  if (!baro.begin()) {
    display.println("Baro Failure");
    display.display();
    while(1);
  }
  // set init altitude
  delay(10000);
  float currentPressure = baro.getPressure();
  baro.setSeaPressure(currentPressure);
  display.println("Init Pressure: ");
  display.println(currentPressure);
  display.display();  
  delay(2000);

}

void loop() {

  myData.pressure      = baro.getPressure();
  myData.altitude_ft   = baro.getAltitude() * 3.28084;
  myData.temperature_f = baro.getTemperature() * 9.0000 / 5.0000 + 32.0000;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(receiverAddress, (uint8_t *) &myData, sizeof(myData));

  delay(2000);
}