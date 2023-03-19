#include <TFT_eSPI.h>

#define JOYSTICK_BUTTON_PIN 34

// initialize screen module
TFT_eSPI tft = TFT_eSPI();

// define constants for screen dimensions and player/rectangle sizes
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;
const int PLAYER_WIDTH = 40;
const int PLAYER_HEIGHT = 10;
const int RECTANGLE_WIDTH = 10;
const int RECTANGLE_HEIGHT = 10;

// define variables for player position and score
int playerX = SCREEN_WIDTH / 2;
int playerY = SCREEN_HEIGHT - PLAYER_HEIGHT - 10;
int score = 0;
int playerLives = 1;

// define variables for joystick pins and thresholds
const int JOYSTICK_X_PIN = A7;
const int JOYSTICK_Y_PIN = A6;
const int JOYSTICK_X_THRESHOLD = 15; // increased because it keeps drifting too much
const int JOYSTICK_Y_THRESHOLD = 10;

// define variables for falling rectangle position and speed
int rectangleX = 0;
int rectangleY = 0;
int rectangleSpeed = 30;
int playerSpeed = 30;

// game state
int gameOver = false;

int resetCount = 0;


  void setup() {
    // initialize screen
    tft.init();
    tft.setRotation(1);
    pinMode(JOYSTICK_BUTTON_PIN, INPUT);
    digitalWrite(JOYSTICK_BUTTON_PIN, HIGH);
  }

  void loop() {
  // Clear the screen
  tft.fillScreen(TFT_BLACK);
  
  // read input from joystick and move player
  int joystickX = analogRead(JOYSTICK_X_PIN);
  int joystickY = analogRead(JOYSTICK_Y_PIN);
  
  // Draw the player
  tft.fillRoundRect(playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT, 5, TFT_BLUE);

  // Update the player position based on joystick input
  if (joystickX < 512 - JOYSTICK_X_THRESHOLD) {
    playerX = max(playerX - playerSpeed, 0);
  }
  else if (joystickX > 512 + JOYSTICK_X_THRESHOLD) {
    playerX = min(playerX + playerSpeed, SCREEN_WIDTH - PLAYER_WIDTH);
  }

  // Draw the falling rectangle
  tft.fillRoundRect(rectangleX, rectangleY, RECTANGLE_WIDTH, RECTANGLE_HEIGHT, 5, TFT_RED);

  // Update the falling rectangle position
  rectangleY += rectangleSpeed;
  if (rectangleY >= SCREEN_HEIGHT) {
    if (rectangleX + RECTANGLE_WIDTH >= playerX && rectangleX <= playerX + PLAYER_WIDTH) {
      score++;
      rectangleX = random(0, SCREEN_WIDTH - RECTANGLE_WIDTH);
      rectangleY = 0;
    }
    else {
      gameOver = true;
    }
  }

  // Display the score
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.print("Score: ");
  tft.print(score);
  tft.print("            Player Lives");
  tft.print(playerLives);

  // Check for game over
  if (gameOver) {
    // End the game
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 - 10);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.print("Game Over");
    tft.setCursor(SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 + 10);
    tft.print("Final Score: ");
    tft.print(score);
    while (true) {
      // Loop indefinitely
    }
  }

  // Read the pin for the button in the joystick, and if pressed add one to resetCount. Otherwise, reset it to 0.
  if (digitalRead(JOYSTICK_BUTTON_PIN) == 0) resetCount += 1;
  else resetCount = 0;

  // If the button has been held for around a second, reset the canvas.
  if (resetCount > 1) {
    tft.fillScreen(TFT_WHITE);
    playerLives = 1; // reset player lives
    score = 0; // reset player score
  }

  // Wait a short time before repeating the loop
  delayMicroseconds(1);
  }