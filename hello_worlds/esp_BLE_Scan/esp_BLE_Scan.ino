#include <NimBLEDevice.h>

const int SCAN_TIME = 3;
int newRSSI = -100;

void performScan() {
    NimBLEScan* pBLEScan = NimBLEDevice::getScan();
    pBLEScan->setActiveScan(false);
    pBLEScan->start(SCAN_TIME, false);
    
    NimBLEScanResults results = pBLEScan->getResults();
    for (int i = 0; i < results.getCount(); i++) {
        NimBLEAdvertisedDevice device = results.getDevice(i);
        
        if (device.haveName() && device.getName() == "Lee") {
            newRSSI = device.getRSSI();
            Serial.printf("Found Lee's device. RSSI: %d\n", newRSSI);
            break; // Exit loop once found
        }
    }
    pBLEScan->clearResults(); // Clear results for next scan
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Hello!");
    NimBLEDevice::init("");
    Serial.println("BLE initialized, ready to scan.");
}

void loop() {
    performScan();
    delay(5000);
}