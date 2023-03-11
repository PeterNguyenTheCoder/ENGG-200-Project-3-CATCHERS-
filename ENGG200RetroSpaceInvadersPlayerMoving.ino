#include <TFT_eSPI.h>

//create the screen variable from the library
TFT_eSPI tft = TFT_eSPI();

// Set up variables for the cursor and counter. Cursor starts in the middle of the screen.
float cursorX = 240.0;
float cursorY = 160.0;
int resetCount = 0;

// Setting the joystick pins here so we can easily change them
#define JOYSTICK_X_PIN A7
#define JOYSTICK_Y_PIN A6
#define JOYSTICK_BUTTON_PIN 34

// Define colours in 4-digit hex                                  
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

void setup() {

  // Initalize the screen and set the orientation correctly, then make sure it's clear.
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(BLACK);

  // Set the cursor to the top left of the screen and print some text.
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("Sketch!");

  // Tell the arduino that the joystick button is on pin 9 and write it high so it gets set to 0 when pushed.
  // If you'd like to learn more about this check out the Elegoo documentation on the Joystick.
  pinMode(JOYSTICK_BUTTON_PIN, INPUT);
  digitalWrite(JOYSTICK_BUTTON_PIN, HIGH);
}

void loop() {
  // Read input and map change the range from 0 - 1023 to -0.05 to 0.05. map() only creates integers. 
  float JoyStickX = map(analogRead(JOYSTICK_X_PIN), 0, 1023, -1000, 1000)/2500.0; //50 is speed variable
  float JoyStickY = map(analogRead(JOYSTICK_Y_PIN), 0, 1023, -1000, 1000)/20000.0;

  // Update cursor position
  cursorY = cursorY + JoyStickY;
  cursorX = cursorX + JoyStickX;

  // Limit the cursor to the bounds of the screen
  cursorX = constrain(cursorX, 0,480);
  cursorY = constrain(cursorY, 0,320);

  // Read the pin for the button in the joystick, and if pressed add one to resetCount. Otherwise, reset it to 0.
  if (digitalRead(JOYSTICK_BUTTON_PIN) == 0) resetCount += 1;
  else resetCount = 0;

  // If the button has been held for around a second, reset the canvas.
  if (resetCount > 1000) {
    tft.fillScreen(BLACK);

    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.println("Sketch!");

    cursorX = 240.0;
    cursorY = 160.0;
  }

  // Finally, draw a pixel where the cursor currently is 
  tft.drawRect(int(cursorX), 300, 20, 4, GREEN);
  tft.fillRect(int(cursorX), 300, 20, 4, GREEN);

  tft.drawRect(int(cursorX-20), 300, 20, 4, BLACK); //erase pixels beside
  tft.drawRect(int(cursorX+20), 300, 20, 4, BLACK); //erase pixels beside
}
