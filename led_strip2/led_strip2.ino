#include <Adafruit_NeoPixel.h>

#define PIN 5 // LED strip pin
#define BUTTON_PIN 10 // Button pin
#define NUM_PATTERN 4
#define NUMPIXELS 150
#define BRIGHTNESS 25 // max = 255

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Button state variables
int buttonState = HIGH;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 200;

// Pattern state
int currentPattern = 0;

// Function to check if button was pressed with debouncing
bool checkButton() {
  int reading = digitalRead(BUTTON_PIN);
  bool buttonPressed = false;
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      
      // If button is pressed (LOW with pull-up)
      if (buttonState == LOW) {
        buttonPressed = true;
        currentPattern = (currentPattern + 1) % NUM_PATTERN;  // Cycle through patterns
        clearStrip();  // Clear strip when changing patterns
      }
    }
  }
  
  lastButtonState = reading;
  return buttonPressed;
}

void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show(); // Initialize all pixels to 'off'
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Enable internal pull-up resistor
}

void loop() {
  checkButton();  // Check button state
  
  // Run the current pattern
  switch (currentPattern) {
    case 0:  // All off
      break;
    case 1:
      rainbow();
      break;
    case 2:
      halloweenRace();
      break;
    case 3:
      halloweenFade();
      break;      
    default:
      clearStrip();
      break;
  }
}

void clearStrip() {
  setAllPixels(0, 0, 0);
  strip.show();
}

void rainbow() {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {

    if (checkButton()) {
      return;
    }
    
    for(int i=0; i<NUMPIXELS; i++) {
      int pixelHue = firstPixelHue + (i * 65536L / NUMPIXELS);
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    delay(10);
  }
}

void setAllPixels(int r, int g, int b) {
  int color = strip.gamma32(strip.Color(r, g, b));
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, color);
  }
}

bool fadeToColorAndBack(int r, int g, int b) {
    // Fade from black to color
    for (int i = 0; i <= 255; i += 1) {
        if (checkButton()) {
            return false;
        }
        float factor = i / 255.0;
        setAllPixels(r * factor, g * factor, b * factor);
        strip.show();
        delay(1);
    }

    // Break up delay into chunks
    for (int i = 0; i < 500; i++) {
        if (checkButton()) {
            return false;
        }
        delay(10);
    }

    // Fade from color to black
    for (int i = 255; i >= 0; i--) {
        if (checkButton()) {
            return false;
        }
        float factor = i / 255.0;
        setAllPixels(r * factor, g * factor, b * factor);
        strip.show();
        delay(1);
    }
    return true;
}

void halloweenFade() {
    // Fade to purple and back
    if (!fadeToColorAndBack(255, 0, 255)) {
        return;  // Exit if button was pressed
    }
    
    // Fade to orange and back
    if (!fadeToColorAndBack(255, 128, 0)) {
        return;  // Exit if button was pressed
    }
}


void halloweenRace() {
    const int groupSize = 5;
    
    // moves the offset through all positions
    for(int offset = 0; offset < NUMPIXELS; offset++) {

        // sets all pixels with offset
        for(int i = 0; i < NUMPIXELS; i++) {
            int position = (i + offset) % (groupSize * 2);
            
            if(position < groupSize) {
                strip.setPixelColor(i, strip.Color(255, 0, 255));  // Purple
            } else {
                strip.setPixelColor(i, strip.Color(255, 60, 0));  // Orange
            }
        }
        strip.show();

        // Delay with button check
        for(int i = 0; i < 50; i++) {
            if (checkButton()) {
                return;
            }
            delay(10);
        }
    }
}