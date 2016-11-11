/*
   arduboy-game, Modus Create 2016
*/

#include "Arduboy.h"
#include "globals.h"
#include "player.h"
#include "bullet.h"
#include "bitmaps.h"

// TODO highScore should be replaced with table in EEPROM
unsigned int score, highScore = 0;
byte livesRemaining = 4;

// Bullets array - We may need a playerBullets and enemyBullets at some point and a MAX global int for each
Bullet bullets[20];

// General purpose text buffer for string concatenations etc
char textBuf[15];

Player spaceShip;

void printText(char *message, int x, int y, int textSize) {
  arduboy.setCursor(x, y);
  arduboy.setTextSize(textSize);
  arduboy.print(message);
}

void introScreen() {
  arduboy.clear();
  draw(24, 4, modusLogo, 0);
  arduboy.display();
  delay(3000);
}

byte titleScreen() {
  byte selectedItem = TITLE_PLAY_GAME;
  unsigned short totalDelay = 0;
  long lastDebounceTime = 0;  // the last time the button was pressed
  long debounceDelay = 100;

  arduboy.clear();
  printText("TITLE", 25, 20, 2);
  arduboy.drawRect(2, 47, 26, 13, 1);
  printText("PLAY", 3, 50, 1);
  printText("CREDITS", 32, 50, 1);
  printText("SETTINGS", 78, 50, 1);
  // TODO DRAW RECT
  arduboy.display();

  while (totalDelay < ATTRACT_MODE_TIMEOUT) {

    if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) {
      break;
    }

    if (arduboy.pressed(LEFT_BUTTON)) {
      if ( (millis() - lastDebounceTime) > debounceDelay) {
        selectedItem = titleMenuLeftButton(selectedItem);
        lastDebounceTime = millis(); //set the current time
      }
    }

    if (arduboy.pressed(RIGHT_BUTTON)) {
      if ( (millis() - lastDebounceTime) > debounceDelay) {
        selectedItem = titleMenuRightButton(selectedItem);
        lastDebounceTime = millis(); //set the current time
      }
    }

    delay(15);
    totalDelay += 15;
  }
  return (totalDelay >= ATTRACT_MODE_TIMEOUT ? TITLE_TIMEOUT : selectedItem);
}

byte titleMenuLeftButton(byte selectedItem) {
  /**
     Handle clicks on the left button
     to navigate through main menu
     items.
  */
  switch (selectedItem) {

    case TITLE_SETTINGS:
      arduboy.drawRect(76, 47, 51, 13, 0);
      arduboy.drawRect(30, 47, 45, 13, 1);
      arduboy.display();
      return TITLE_CREDITS;
      break;

    case TITLE_CREDITS:
      arduboy.drawRect(30, 47, 45, 13, 0);
      arduboy.drawRect(2, 47, 26, 13, 1);
      arduboy.display();
      return  TITLE_PLAY_GAME;
      break;

    default: break;
  }
}


byte titleMenuRightButton(byte selectedItem) {
  /**
     Handle clicks on the right button
     to navigate through main menu
     items.
  */
  switch (selectedItem) {

    case TITLE_PLAY_GAME:
      arduboy.drawRect(2, 47, 26, 13, 0);
      arduboy.drawRect(30, 47, 45, 13, 1);
      arduboy.display();
      return TITLE_CREDITS;
      break;

    case TITLE_CREDITS:
      arduboy.drawRect(30, 47, 45, 13, 0);
      arduboy.drawRect(76, 47, 51, 13, 1);
      arduboy.display();
      return TITLE_SETTINGS;
      break;

    default: break;
  }
}

void highScoreScreen() {
  // TODO, this is placeholder
  arduboy.clear();
  printText("HI SCORES", 3, 15, 2);
  arduboy.display();
  delay(3000);
}

void creditsScreen() {
  // TODO, this is placeholder
  arduboy.clear();
  printText("CREDITS", 20, 25, 2);
  arduboy.display();
  delay(5000);
}

void settingsScreen() {
  // TODO, this is a placeholder
  long lastDebounceTime = 0;  // the last time the button was pressed
  long debounceDelay = 100;
  bool exit_settings_menu = false;
  byte selectedItem;

  arduboy.clear();
  printText("SETTINGS", 20, 5, 2);
  printText("SOUND", 20, 25, 1);
  printText("RESET HIGHSCORE", 20, 37, 1);
  printText("EXIT", 20, 49, 1);
  arduboy.drawRect(17, 22, 35, 13, 1);
  arduboy.display();


  while (!exit_settings_menu) {

    if (arduboy.pressed(DOWN_BUTTON)) {
      if ( (millis() - lastDebounceTime) > debounceDelay) {
        selectedItem = settingMenuDownButton(selectedItem);
        lastDebounceTime = millis(); //set the current time
      }
    }


    if (arduboy.pressed(UP_BUTTON)) {
      if ( (millis() - lastDebounceTime) > debounceDelay) {
        selectedItem = settingMenuUpButton(selectedItem);
        lastDebounceTime = millis(); //set the current time
      }
    }

    if (arduboy.pressed(A_BUTTON)) {
      if ( (millis() - lastDebounceTime) > debounceDelay) {
        switch (selectedItem) {

          case SETTINGS_EXIT:
            exit_settings_menu = true;
            break;

          default: break;
        }
      }
      lastDebounceTime = millis(); //set the current time
    }
  }
}


byte settingMenuDownButton(byte selectedItem) {
  /**
     Handle clicks on the right button
     to navigate through main menu
     items.
  */
  switch (selectedItem) {

    case SETTINGS_SOUND:
      arduboy.drawRect(17, 22, 35, 13, 0);
      arduboy.drawRect(17, 34, 95, 13, 1);
      arduboy.display();
      return SETTINGS_RESET_HIGH_SCORE;
      break;

    case SETTINGS_RESET_HIGH_SCORE:
      arduboy.drawRect(17, 34, 95, 13, 0);
      arduboy.drawRect(17, 46, 29, 13, 1);
      arduboy.display();
      return SETTINGS_EXIT;
      break;

    default: break;
  }
}


byte settingMenuUpButton(byte selectedItem) {
  /**
     Handle clicks on the right button
     to navigate through main menu
     items.
  */
  switch (selectedItem) {

    case SETTINGS_EXIT:
      arduboy.drawRect(17, 46, 29, 13, 0);
      arduboy.drawRect(17, 34, 95, 13, 1);
      arduboy.display();
      return SETTINGS_RESET_HIGH_SCORE;
      break;

    case SETTINGS_RESET_HIGH_SCORE:
      arduboy.drawRect(17, 34, 95, 13, 0);
      arduboy.drawRect(17, 22, 35, 13, 1);
      arduboy.display();
      return SETTINGS_SOUND;
      break;

    default: break;
  }
}


void playGame() {
  // TODO, this is placeholder, should also use livesRemaining
  // to count down user lives
  score = 0;

  // Random test to set score
  unsigned int randomScore = random(65000, 99999);

  // Loop to simulate a game that ends with score being
  // close to value of randomScore
  while (score < randomScore) {
    arduboy.clearDisplay();
    sprintf(textBuf, "SCORE %u", score);
    printText(textBuf, 0, 0, 1);
    score += random(0, 50);

    drawPlayerShip();

    bullets[0].draw();
    bullets[0].update();

    arduboy.display();
  }
}

void drawPlayerShip() {
  if (arduboy.pressed(RIGHT_BUTTON) && (spaceShip.x < MAX_SHIP_X)) {
    spaceShip.x++;
  }

  if (arduboy.pressed(LEFT_BUTTON) && (spaceShip.x > MIN_SHIP_X)) {
    spaceShip.x--;
  }

  if (arduboy.pressed(UP_BUTTON)) {
    if (spaceShip.y > MIN_SHIP_X) {
      spaceShip.y--;
    }
    if (arduboy.everyXFrames(9)) {
      spaceShip.frame--;
    }
    if (spaceShip.frame < 0) {
      spaceShip.frame = 0;
    }
  }

  if (arduboy.pressed(DOWN_BUTTON)) {
    if (spaceShip.y < MAX_SHIP_Y)  {
      spaceShip.y++;
    }
    if (arduboy.everyXFrames(9)) {
      spaceShip.frame++;
    }
    if (spaceShip.frame  > 4) {
      spaceShip.frame = 4;
    }
  }

  if (arduboy.pressed(A_BUTTON)) {
    bullets[0].set(spaceShip.x, spaceShip.y + (spaceShip.height / 2) - 1);
  }

  if (arduboy.notPressed(UP_BUTTON) && arduboy.notPressed(DOWN_BUTTON)) {
    if (arduboy.everyXFrames(12)) {
      if (spaceShip.frame > 2) {
        spaceShip.frame--;
      }
      if (spaceShip.frame < 2) {
        spaceShip.frame++;
      }
    }
  }

  draw(spaceShip.x, spaceShip.y, playerShip, spaceShip.frame);
}

void draw(int x, int y, const uint8_t *bitmap, uint8_t frame) {
  unsigned int frame_offset;
  uint8_t width = pgm_read_byte(bitmap);
  uint8_t height = pgm_read_byte(++bitmap);

  bitmap++;
  if (frame > 0) {
    frame_offset = (width * ( height / 8 + ( height % 8 == 0 ? 0 : 1)));
    // sprite plus mask uses twice as much space for each frame
    bitmap += frame * frame_offset;
  }
  arduboy.drawBitmap(x, y, bitmap, width, height, 1);
}

void gameOverScreen() {
  // TODO, this is placeholder
  arduboy.clear();
  printText("GAME OVER", 15, 30, 2);
  arduboy.display();
  delay(3000);
}

void newHighScoreScreen() {
  // TODO, this is placeholder
  arduboy.clear();
  printText("NEW HI", 4, 25, 2);
  arduboy.display();
  delay(3000);
}

// Initialization runs once only
void setup() {
  arduboy.beginNoLogo();
  introScreen();
  spaceShip.set();
}


// Main program loop
void loop() {
  byte result;

  // TODO alternate between titleScreen and highScoreScreen on a timer
  // until user pressed a button
  result = titleScreen();

  switch (result) {
    case TITLE_CREDITS:
      creditsScreen();
      break;
    case TITLE_PLAY_GAME:
      playGame();
      gameOverScreen();
      // TODO high score should be checked against a set of high scores
      // in the EEPROM
      if (score > highScore) {
        newHighScoreScreen();
        highScore = score;
      }

      highScoreScreen();
      break;
    case TITLE_SETTINGS:
      settingsScreen();
      break;
    case TITLE_TIMEOUT:
      // No button pressed on title, alternate with high score
      highScoreScreen();
      break;
  }
}
