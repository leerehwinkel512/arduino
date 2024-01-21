
#include "WiFiS3.h"
#include "WiFiSSLClient.h"
#include "IPAddress.h"
#include <ArduinoHttpClient.h>

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#include <Arduino_JSON.h>

#include "arduino_secrets.h" 

char ssid[]     = SECRET_SSID;
char pass[]     = SECRET_PASS;
char api_auth[] = API_AUTH;
char http[]     = HTTP;

int status = WL_IDLE_STATUS;

WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, http 443);


// led croll config
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN     13
#define DATA_PIN    11
#define CS_PIN      10

#define	BUF_SIZE 200
char curMessage[BUF_SIZE] = { "Start" };
char newMessage[BUF_SIZE] = { "" };
bool newMessageAvailable = false;

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

String guid;
String llm_answer;

void setup() {

  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // led scroll
  P.begin();
  P.displayText(curMessage, PA_LEFT, 25, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
  
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }
  
  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network.
    status = WiFi.begin(ssid, pass);
     
    // wait 10 seconds for connection:
    delay(10000);
  }
  
  printWifiStatus();


  // get existing state
  Serial.println();
  Serial.println("making GET request for START State");

  client.beginRequest();
  client.post("/askLLM");
  client.sendHeader("Content-Type", "application/json");
  client.sendHeader("password", API_AUTH);
  client.sendHeader("Connection", "close");
  client.endRequest();
  
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("GET Status code: ");
  Serial.println(statusCode);
  Serial.print("GET Response: ");
  Serial.println(response);
  
  Serial.println("Disconnecting from server.");
  client.stop();

  JSONVar responseJSON = JSON.parse(response);

  guid = (const char*)responseJSON["Guid"];
  llm_answer = (const char*)responseJSON["LLM_Message"];

  Serial.print("Guid: ");
  Serial.println(guid);
  Serial.print("LLM_Message: ");
  Serial.println(llm_answer);
  Serial.println();  
}

unsigned long previousMillis = 0;
const long interval = 20000;  

void loop() {

  unsigned long currentMillis = millis();

  // check for new message
  if (currentMillis - previousMillis >= interval){

    previousMillis = currentMillis;

    // get current state
    Serial.println("making GET request for POLLING State");

    client.beginRequest();
    client.post("/askLLM");
    client.sendHeader("Content-Type", "application/json");
    client.sendHeader("password", API_AUTH);
    client.sendHeader("Connection", "close");
    client.endRequest();
    
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();

    Serial.print("GET Status code: ");
    Serial.println(statusCode);
    Serial.print("GET Response: ");
    Serial.println(response);
    
    Serial.println("Disconnecting from server.");
    client.stop();

    JSONVar responseJSON = JSON.parse(response);

    String new_guid = (const char*)responseJSON["Guid"];
    llm_answer = (const char*)responseJSON["LLM_Message"];

    Serial.print("Guid: ");
    Serial.println(new_guid);
    Serial.print("LLM_Message: ");
    Serial.println(llm_answer);
    Serial.println();

    if (new_guid != guid){
      Serial.println("New Message Arrived: ");
      Serial.println();
      newMessageAvailable = true;
      llm_answer.toCharArray(newMessage, BUF_SIZE);
      guid = new_guid;
    }
  }

  P.displayAnimate();

  if (newMessageAvailable) {
    Serial.println("Update message.");
    P.displayClear();
    strcpy(curMessage, newMessage);
    newMessageAvailable = false;
    P.displayReset();
  }

}


void printWifiStatus() {

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
