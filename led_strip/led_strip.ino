#include <Adafruit_NeoPixel.h>

#define PIN 5 // D2
#define NUMPIXELS 150
#define BRIGHTNESS 25 // max = 255

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  
  halloweenEffect();

  rainbow();

}

void rainbow() {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
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

void fadeToColorAndBack(int r, int g, int b) {
  // Fade from black to color
  for (int i = 0; i <= 255; i += 1) {
    float factor = i / 255.0;
    setAllPixels(r * factor, g * factor, b * factor);
    strip.show();
    delay(1);
  }

  delay(5000);
  
  // Fade from color to black
  for (int i = 255; i >= 0; i--) {
    float factor = i / 255.0;
    setAllPixels(r * factor, g * factor, b * factor);
    strip.show();
    delay(1);
  }
}

void halloweenEffect() {
  // Fade to purple and back
  fadeToColorAndBack(255, 0, 255);
  
  // Fade to orange and back
  fadeToColorAndBack(255, 128, 0);
}