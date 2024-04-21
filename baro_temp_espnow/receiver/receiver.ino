#include <esp_now.h>
#include <WiFi.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// Define a data structure
typedef struct struct_message {
  float pressure;
  float altitude_ft;
  float temperature_f;
} struct_message;
struct_message myData;


unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
 
 
// Callback function executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {

  // copy message data to struct
  memcpy(&myData, incomingData, sizeof(myData));

  display.clearDisplay();
  display.setCursor(0, 0);

  display.print("Pressure: ");
  display.println(myData.pressure);

  display.print("Altitude (ft): ");
  display.println(myData.altitude_ft);

  display.print("Temp (f): ");
  display.println(myData.temperature_f);

  display.display();

  previousMillis = currentMillis;
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
}


void loop() {

  currentMillis = millis();

  if (currentMillis - previousMillis >= 10000) {

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Waiting for data...");
    display.display();  
  }
}