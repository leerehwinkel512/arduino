#include "MD_Parola.h"
#include "MD_MAX72xx.h"
#include "SPI.h"

// Hardware configuration
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 16
#define CS_PIN    10
#define DATA_PIN  11
#define CLK_PIN   13

#define SCROLL_SPEED 20
#define PAUSE_REPEAT 0

// Create display instance
MD_Parola display(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// Store the message - Use backslash to continue strings across multiple lines
const char message[] =
"Every great dream begins with a dreamer. Always remember, you have within you the strength, the patience, and the passion to reach for the stars to change the world."
"     "
"- Harriet Tubman, abolitionist"
"                                         "
"Get in good trouble, necessary trouble, and help redeem the soul of America."
"     "
"- John Lewis, congressman"
"                                         "
"Never underestimate the power of dreams and the influence of the human spirit. We are all the same in this notion: The potential for greatness lives within each of us."
"     "
"- Wilma Rudolph, Olympic athlete"
"                                         "
"The time is always right to do what is right."
"     "
"- Dr. Martin Luther King, Jr., civil rights activist"
"                                         "
"When I dare to be powerful, to use my strength in the service of my vision, then it becomes less and less important whether I am afraid."
"     "
"- Audre Lorde, writer"
"                                         "
"The most common way people give up their power is by thinking they don't have any."
"     "
"- Alice Walker, writer"
"                                         "
"Won't it be wonderful when Black history and Native American history and Jewish history and all of U.S. history is taught from one book. Just U.S. history."
"     "
"- Maya Angelou, writer"
"                                         ";

void setup() {
  display.begin();
  display.setIntensity(5);
  display.displayClear();
  
  // Start displaying the text
  display.displayText(message, PA_LEFT, SCROLL_SPEED, PAUSE_REPEAT, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void loop() {
  if (display.displayAnimate()) {
    display.displayReset();
  }
}
