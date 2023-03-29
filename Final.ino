#include <TFT_eSPI.h>

#define JOYSTICK_BUTTON_PIN 34

// initialize screen module
TFT_eSPI tft = TFT_eSPI();

// battery sensor variables



// define constants for screen dimensions and player/rectangle sizes
const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 320;
const int PLAYER_WIDTH = 40;
const int PLAYER_HEIGHT = 10;
const int RECTANGLE_WIDTH = 10;
const int RECTANGLE_HEIGHT = 10;

// define variables for player position and score
int playerX = SCREEN_WIDTH / 2;
int playerY = SCREEN_HEIGHT - PLAYER_HEIGHT - 10;
int score = 0;
int playerLives = 3;

// define variables for joystick pins and thresholds
const int JOYSTICK_X_PIN = A7;
const int JOYSTICK_Y_PIN = A6;
const int JOYSTICK_X_THRESHOLD = 15; // increased because it keeps drifting too much
const int JOYSTICK_Y_THRESHOLD = 10;

// define variables for falling rectangle position and speed
int rectangleX = 0;
int rectangleY = 0;
int playerSpeed = 8;
float rectangleSpeed = 5;

// Drawing Variables
int checkLives;
int checkScore;

// game state
int gameOver = false;

int resetCount = 0;

int b = 0; //0 = false, 1 = true

void setup() {
  // initialize screen
  tft.init();
  tft.setRotation(1);
  pinMode(JOYSTICK_BUTTON_PIN, INPUT); // 0 = button is pressed, 1 = button is not pressed
  digitalWrite(JOYSTICK_BUTTON_PIN, HIGH); 
}

void endMenu() {
    // End the game
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 - 10);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.print("Game Over");
  tft.setCursor(SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 + 10);
  tft.print("Final Score: ");
  tft.print(score);
  while (true){
    if (digitalRead(JOYSTICK_BUTTON_PIN) == 0) {
      break;
    }
  }
}

void startMenu() {
    score = 0;
    playerLives = 3;
    rectangleSpeed = 5;
    int playerX = SCREEN_WIDTH / 2;
    int playerY = SCREEN_HEIGHT - PLAYER_HEIGHT - 10;
    rectangleX = random(0, SCREEN_WIDTH - RECTANGLE_WIDTH);
    rectangleY = 0;
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(4);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(SCREEN_WIDTH / 5 + 40, SCREEN_HEIGHT / 5 + 10);
    tft.print("CATCHER!");
    tft.setCursor(0, 130);
    tft.setTextSize(2);
    tft.println("Catch the red balls with your blue board ");
    tft.println("to earn points, control your board ");
    tft.println("using the joystick. Reset the game ");
    tft.println("anytime by pressing down on the joystick.");
    tft.setCursor(0, 220);
    tft.print("Have fun, and don't let the balls drop!");
    tft.setTextSize(2);
    tft.setCursor(0, SCREEN_HEIGHT / 5 + 200);
    tft.print("Press Joystick To Play.....");
    while (true){
      if (digitalRead(JOYSTICK_BUTTON_PIN) == 0){
        b = 1;
        break;
    gameOver = false;
    }
  }
}

void ballFall(){
  // Erase the falling rectangle
  tft.fillRoundRect(rectangleX, rectangleY, RECTANGLE_WIDTH, RECTANGLE_HEIGHT, 5, TFT_BLACK);  
  checkLives = playerLives; // var for drawing lives
  checkScore = score;
  // Update the falling rectangle position
  rectangleY += rectangleSpeed;
  if (rectangleY >= SCREEN_HEIGHT - 20) { // check to see if rectangle has hit the bottom of the screen
    if (rectangleX + RECTANGLE_WIDTH >= playerX && rectangleX <= playerX + PLAYER_WIDTH) {
      score++;
      rectangleX = random(0, SCREEN_WIDTH - RECTANGLE_WIDTH);
      rectangleY = 0;
    }
    else {
      rectangleX = random(0, SCREEN_WIDTH - RECTANGLE_WIDTH);
      rectangleY = 0;
      playerLives += -1;
      rectangleSpeed -= -2;
      if (playerLives == 0){
        gameOver = true;
      }
      
    }

  
  }

  // Draw the falling rectangle
  tft.fillRoundRect(rectangleX, rectangleY, RECTANGLE_WIDTH, RECTANGLE_HEIGHT, 5, TFT_RED);

}


void loop() {

  b = 0;    
  startMenu();
  delay (1000);
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 9V):
  float voltage = sensorValue * (9.0 / 1023.0);
  float batteryPercentage = voltage / 9.0 * 100;
  float initialBattery;
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(150, 0);
  tft.setTextColor(TFT_WHITE);
  tft.print(batteryPercentage);
  tft.print("%");
  tft.setCursor(0, 0);
  
   


  while (true){

    // battery variables


    if (playerLives == 3){
      rectangleSpeed = rectangleSpeed;
    }

    if (digitalRead(JOYSTICK_BUTTON_PIN) == 0) {
      endMenu();
      b = 0;
      break;
    }
    else {

      // read input from joystick and move player
      int joystickX = analogRead(JOYSTICK_X_PIN);
      int joystickY = analogRead(JOYSTICK_Y_PIN);

      // Erase the Player

      tft.fillRoundRect(playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT, 5, TFT_BLACK);

      // Update the player position based on joystick input
      if (joystickX < 512 - JOYSTICK_X_THRESHOLD) {
        playerX = max(playerX - playerSpeed, 0);
      }
      else if (joystickX > 512 + JOYSTICK_X_THRESHOLD) {
        playerX = min(playerX + playerSpeed, SCREEN_WIDTH - PLAYER_WIDTH);
      }
      
      // Draw the player
      tft.fillRoundRect(playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT, 5, TFT_BLUE);


      
      ballFall();

      int previousScore = score - 1;
      int previousLives = playerLives + 1;

      // erase the score, lives, battery
     
      tft.setTextColor(TFT_BLACK);
      tft.setTextSize(2);
      tft.setCursor(80, 0);
      if (score != checkScore){
        tft.print(previousScore);
      }
      tft.setCursor(420, 0);
      if (playerLives != checkLives){
        tft.print(previousLives);
      }
      tft.setCursor(200, 0);
      if (initialBattery != batteryPercentage){
        tft.print(initialBattery);
      }

      

      // Display the score, lives, battery
      tft.setCursor(0, 0);
      tft.setTextColor(TFT_WHITE);
      tft.setTextSize(2);
      tft.print("Score: ");
      tft.setCursor(80, 0);
      tft.print(score);
      tft.setCursor(0, 0);
      tft.print("                      Player Lives: ");
      tft.setCursor(420, 0);
      tft.print(playerLives);

      

      // Check for game over
      if (gameOver) { 
        gameOver = false;        
        endMenu();
        break;

      }

      // Read the pin for the button in the joystick, and if pressed add one to resetCount. Otherwise, reset it to 0.
      if (digitalRead(JOYSTICK_BUTTON_PIN) == 0){
        resetCount += 1;
      }
      else{
        resetCount = 0;
      }


      // If the button has been held for around a second, reset the canvas.
      if (resetCount > 1) {
        tft.fillScreen(TFT_WHITE);
        playerLives = 3; // reset player lives
        score = 0; // reset player score
      }

      // Wait a short time before repeating the loop
      delay(1);
    }
  }
  }

