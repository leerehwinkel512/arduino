#include <GxEPD2_BW.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <EEPROM.h>

// Pin configuration
#define EPD_CS    10
#define EPD_DC    9
#define EPD_RST   8
#define EPD_BUSY  7

// Button pins
#define BTN_HENRY  2
#define BTN_TURNER 3
#define BTN_FINN   4

// EEPROM addresses
#define ADDR_MAGIC  0   // Magic number to detect first run
#define ADDR_HENRY  4
#define ADDR_TURNER 8
#define ADDR_FINN   12

#define EEPROM_MAGIC 0xBE57  // "BEST" - indicates EEPROM is initialized

int partialRefreshCount = 0;
const int FULL_REFRESH_INTERVAL = 5;  // Full refresh every 5 updates

// DEPG0290BS driver
GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> display(GxEPD2_290_BS(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

// Scores
int henryScore = 0;
int turnerScore = 0;
int finnScore = 0;

// Button handling
struct ButtonState {
  int pin;
  int* score;
  int addrEEPROM;
  const char* name;
  bool lastState;
  bool currentState;
  unsigned long pressStartTime;
  unsigned long lastDebounceTime;
  bool actionTaken;        // Have we already done an action for this press?
  bool decrementMode;      // Are we in decrement (hold) mode?
};

ButtonState buttons[3];

const unsigned long DEBOUNCE_DELAY = 50;
const unsigned long HOLD_TIME = 600;  // Hold 600ms to decrement instead

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("Initializing Hugs for Mom...");
  
  // Initialize buttons
  pinMode(BTN_HENRY, INPUT_PULLUP);
  pinMode(BTN_TURNER, INPUT_PULLUP);
  pinMode(BTN_FINN, INPUT_PULLUP);
  
  // Initialize EEPROM (for ESP8266/ESP32, may need EEPROM.begin())
  #if defined(ESP8266) || defined(ESP32)
  EEPROM.begin(16);
  #endif
  
  // Check for full reset: all 3 buttons held on startup
  bool allPressed = (digitalRead(BTN_HENRY) == LOW) && 
                    (digitalRead(BTN_TURNER) == LOW) && 
                    (digitalRead(BTN_FINN) == LOW);
  
  if (allPressed) {
    Serial.println("*** FULL RESET DETECTED ***");
    Serial.println("All buttons held - resetting all scores to zero!");
    
    henryScore = 0;
    turnerScore = 0;
    finnScore = 0;
    
    EEPROM.put(ADDR_MAGIC, (uint16_t)EEPROM_MAGIC);
    EEPROM.put(ADDR_HENRY, henryScore);
    EEPROM.put(ADDR_TURNER, turnerScore);
    EEPROM.put(ADDR_FINN, finnScore);
    
    #if defined(ESP8266) || defined(ESP32)
    EEPROM.commit();
    #endif
    
    // Wait for buttons to be released before continuing
    while ((digitalRead(BTN_HENRY) == LOW) || 
           (digitalRead(BTN_TURNER) == LOW) || 
           (digitalRead(BTN_FINN) == LOW)) {
      delay(50);
    }
    delay(200);  // Extra debounce
  }
  
  // Check if EEPROM has been initialized (and we didn't just reset)
  uint16_t magic;
  EEPROM.get(ADDR_MAGIC, magic);
  
  if (!allPressed && magic != EEPROM_MAGIC) {
    // First run - initialize EEPROM
    Serial.println("First run detected, initializing EEPROM...");
    henryScore = 0;
    turnerScore = 0;
    finnScore = 0;
    
    EEPROM.put(ADDR_MAGIC, (uint16_t)EEPROM_MAGIC);
    EEPROM.put(ADDR_HENRY, henryScore);
    EEPROM.put(ADDR_TURNER, turnerScore);
    EEPROM.put(ADDR_FINN, finnScore);
    
    #if defined(ESP8266) || defined(ESP32)
    EEPROM.commit();
    #endif
  } else if (!allPressed) {
    // Load existing scores
    EEPROM.get(ADDR_HENRY, henryScore);
    EEPROM.get(ADDR_TURNER, turnerScore);
    EEPROM.get(ADDR_FINN, finnScore);
    
    // Sanity check (shouldn't be needed with magic number, but just in case)
    if (henryScore < 0 || henryScore > 9999) henryScore = 0;
    if (turnerScore < 0 || turnerScore > 9999) turnerScore = 0;
    if (finnScore < 0 || finnScore > 9999) finnScore = 0;
  }
  
  Serial.print("Loaded scores - Henry: ");
  Serial.print(henryScore);
  Serial.print(", Turner: ");
  Serial.print(turnerScore);
  Serial.print(", Finn: ");
  Serial.println(finnScore);
  
  // Setup button state tracking
  buttons[0] = {BTN_HENRY, &henryScore, ADDR_HENRY, "Henry", true, true, 0, 0, true, false};
  buttons[1] = {BTN_TURNER, &turnerScore, ADDR_TURNER, "Turner", true, true, 0, 0, true, false};
  buttons[2] = {BTN_FINN, &finnScore, ADDR_FINN, "Finn", true, true, 0, 0, true, false};
  
  // Initialize display
  display.init(0);  // 0 = no debug output to serial (quieter)
  display.setRotation(1);
  display.setTextColor(GxEPD_BLACK);
  
  // Initial full refresh display
  updateDisplayFull();
  
  Serial.println("Ready! Press = +1, Hold = -1");
}

// Full refresh - use only on startup
void updateDisplayFull() {
  display.setFullWindow();
  display.firstPage();
  
  do {
    drawScreen();
  } while (display.nextPage());
  
  display.powerOff();
}

// Partial refresh - faster, less flashing
void updateDisplayPartial() {
  partialRefreshCount++;
  
  if (partialRefreshCount >= FULL_REFRESH_INTERVAL) {
    // Do a full refresh to clear ghosting
    partialRefreshCount = 0;
    updateDisplayFull();
    return;
  }
  
  display.setPartialWindow(0, 0, 296, 128);
  display.firstPage();
  
  do {
    drawScreen();
  } while (display.nextPage());
  
  display.powerOff();
}

void drawScreen() {
  display.fillScreen(GxEPD_WHITE);
  
  // Title at top
  display.setFont(&FreeMono12pt7b);
  int16_t tbx, tby; 
  uint16_t tbw, tbh;
  display.getTextBounds("HUGS FOR MOM", 0, 0, &tbx, &tby, &tbw, &tbh);
  display.setCursor((296 - tbw) / 2, 20);
  display.print("HUGS FOR MOM");
  
  // Draw dividing lines
  display.drawLine(98, 30, 98, 128, GxEPD_BLACK);
  display.drawLine(197, 30, 197, 128, GxEPD_BLACK);
  display.drawLine(0, 30, 296, 30, GxEPD_BLACK);
  
  // Draw each child's column
  drawChildScore("HENRY", henryScore, 49);
  drawChildScore("TURNER", turnerScore, 147);
  drawChildScore("FINN", finnScore, 246);
}

void drawChildScore(const char* name, int score, int centerX) {
  // Draw name
  display.setFont(&FreeMono12pt7b);
  int16_t tbx, tby; 
  uint16_t tbw, tbh;
  display.getTextBounds(name, 0, 0, &tbx, &tby, &tbw, &tbh);
  display.setCursor(centerX - tbw/2, 50);
  display.print(name);
  
  // Draw score - BIG
  display.setFont(&FreeSansBold24pt7b);
  String scoreStr = String(score);
  display.getTextBounds(scoreStr.c_str(), 0, 0, &tbx, &tby, &tbw, &tbh);
  display.setCursor(centerX - tbw/2, 95);
  display.print(scoreStr);
  
  // Heart for everyone
  drawHeart(centerX, 110);
}

void drawHeart(int x, int y) {
  display.fillCircle(x - 5, y - 3, 5, GxEPD_BLACK);
  display.fillCircle(x + 5, y - 3, 5, GxEPD_BLACK);
  display.fillTriangle(x - 10, y - 1, x + 10, y - 1, x, y + 10, GxEPD_BLACK);
}

void saveScore(int addr, int score) {
  EEPROM.put(addr, score);
  #if defined(ESP8266) || defined(ESP32)
  EEPROM.commit();
  #endif
}

void handleButtons() {
  unsigned long now = millis();
  
  for (int i = 0; i < 3; i++) {
    ButtonState& btn = buttons[i];
    
    // Read button (LOW = pressed due to INPUT_PULLUP)
    bool reading = digitalRead(btn.pin) == LOW;
    
    // Debounce
    if (reading != btn.lastState) {
      btn.lastDebounceTime = now;
    }
    
    if ((now - btn.lastDebounceTime) > DEBOUNCE_DELAY) {
      // State has been stable long enough
      
      if (reading != btn.currentState) {
        btn.currentState = reading;
        
        if (reading) {
          // Button just pressed
          btn.pressStartTime = now;
          btn.actionTaken = false;
          btn.decrementMode = false;
        } else {
          // Button just released
          if (!btn.actionTaken) {
            // Quick press - INCREMENT
            (*btn.score)++;
            Serial.print(btn.name);
            Serial.print(" +1 -> ");
            Serial.println(*btn.score);
            saveScore(btn.addrEEPROM, *btn.score);
            updateDisplayPartial();
          }
          btn.actionTaken = false;
          btn.decrementMode = false;
        }
      }
      
      // Check for hold while button is still pressed
      if (btn.currentState && !btn.actionTaken) {
        if ((now - btn.pressStartTime) >= HOLD_TIME) {
          // Held long enough - DECREMENT
          if (*btn.score > 0) {
            (*btn.score)--;
            Serial.print(btn.name);
            Serial.print(" -1 (hold) -> ");
            Serial.println(*btn.score);
            saveScore(btn.addrEEPROM, *btn.score);
            updateDisplayPartial();
          }
          btn.actionTaken = true;
          btn.decrementMode = true;
        }
      }
    }
    
    btn.lastState = reading;
  }
}

void loop() {
  handleButtons();
  delay(10);  // Small delay to reduce CPU usage
}
