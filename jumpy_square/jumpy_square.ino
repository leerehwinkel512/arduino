#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <vector>

#define SCREEN_WIDTH 128 // OLED display width
#define SCREEN_HEIGHT 64 // OLED display height
#define OLED_RESET    -1 // No reset pin
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUTTON_PIN 0 // Change this to your button's pin
#define GRAVITY 2 // Gravity effect on the player
#define JUMP_FORCE -12 // Jump force of the player
#define PLAYER_SIZE 5 // Size of the player square
#define OBSTACLE_SIZE 10 // Size of the square obstacle
#define PLAYER_GROUND (SCREEN_HEIGHT - PLAYER_SIZE) // Ground level for the player

struct Obstacle {
  int x, y;
};

std::vector<Obstacle> obstacles;
int playerY = PLAYER_GROUND;
int velocityY = 0;
unsigned long lastObstacleTime = 0;
unsigned long points = 0;

void showStartupLogo() {
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Jumpy");
  display.print("Square");
  
  display.display();
  delay(3000);
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  showStartupLogo();
}

void loop() {
  handleJump();
  updatePlayer();
  updateObstacles();
  checkCollisions();
  updatePoints();
  render();
}

void handleJump() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (playerY == PLAYER_GROUND) {
      velocityY = JUMP_FORCE; // Apply jump force
    }
  }
}

void updatePlayer() {
  playerY += velocityY;
  velocityY += GRAVITY; // Apply gravity

  // Ground collision
  if (playerY > PLAYER_GROUND) {
    playerY = PLAYER_GROUND;
    velocityY = 0;
  }
}

void updatePoints() {
  points++; // Increment points over time
}

void updateObstacles() {
  static unsigned long nextObstacleTime = 0; // When the next obstacle should be added
  unsigned long currentTime = millis();

  if (currentTime >= nextObstacleTime) {
      // Place the top of the obstacle at a position where its bottom aligns with the bottom of the screen
      obstacles.push_back({SCREEN_WIDTH, SCREEN_HEIGHT - OBSTACLE_SIZE});
      nextObstacleTime = currentTime + random(250, 2001); // Random delay between obstacles
  }

  for (int i = 0; i < obstacles.size(); i++) {
    obstacles[i].x -= 2; // Move obstacle to the left

    // Remove obstacle if it goes off screen
    if (obstacles[i].x < -OBSTACLE_SIZE) {
      obstacles.erase(obstacles.begin() + i);
      i--;
    }
  }
}

void checkCollisions() {
  for (Obstacle obs : obstacles) {
    if (playerY < obs.y + OBSTACLE_SIZE && playerY + PLAYER_SIZE > obs.y &&
        SCREEN_WIDTH/2 < obs.x + OBSTACLE_SIZE && SCREEN_WIDTH/2 + PLAYER_SIZE > obs.x) {

      // Collision detected
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println("GAME OVER");
      display.display();

      delay(3000); // delay then restart
      restartGame();
    }
  }
}

void restartGame() {
  obstacles.clear(); // Clear all obstacles
  playerY = PLAYER_GROUND; // Reset player position
  velocityY = 0; // Reset player velocity
  points = 0; // Reset points
  lastObstacleTime = millis(); // Reset the obstacle timer
}

void render() {
  display.clearDisplay();
  display.fillRect(SCREEN_WIDTH/2 - PLAYER_SIZE/2, playerY, PLAYER_SIZE, PLAYER_SIZE, WHITE); // Draw player

  for (Obstacle obs : obstacles) {
      // Draw each obstacle such that its bottom aligns with the bottom of the screen
      display.fillRect(obs.x - OBSTACLE_SIZE/2, obs.y, OBSTACLE_SIZE, OBSTACLE_SIZE, WHITE);
  }

  // Display points
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(SCREEN_WIDTH - 30, 0); // Position the text at the top-right corner
  display.print(points);  

  display.display();
}
