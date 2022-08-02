#include <ArduboyTonesPitches.h>
#include <ArduboyTones.h>
#include <Arduboy2.h>
#include <EEPROM.h>
#include "sprites.h"
#include "tones.h"

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

uint8_t menuItemsTitle = 3;
uint8_t menuItemsMenu = 4;
uint8_t menuItemsMenu2 = 4;
uint8_t GameStart = 1;
bool sfxToggle = true;
uint32_t money = 50;

uint8_t dice = 0;
uint8_t didDiceRoll = 0;
uint32_t delayDice = 0;
bool diceRestart = false;

uint8_t cursorIndexTitle = 0;
uint8_t cursorTitleX = 88;
uint8_t cursorTitleY = 25;

uint8_t cursorIndexMenu = 0;
uint8_t cursorMenuX = 88;
uint8_t cursorMenuY = 25;

uint8_t cursorIndexMenu2 = 0;
uint8_t cursorMenu2X = 88;
uint8_t cursorMenu2Y = 25;

enum class GameState : uint8_t {
  GameTitle,
  GameOptions,
  GameCredits,
  GameStart,
  GameMenu,
  GameWork,
  GameBuy,
  GameSell,
  GameMenu2,
  GameBus,
  GameDice,
  GameDiceDelay,
  GameBank,
  GameProperties,
};

GameState gameState = GameState::GameTitle;

void setup()
{
  arduboy.begin();
  arduboy.initRandomSeed();
}

void loop()
{
  if(!arduboy.nextFrame())
    return;

  arduboy.pollButtons();
  arduboy.clear();
  gameLoop();
  arduboy.display();
}

void gameLoop()
{
  switch(gameState)
  {
    case GameState::GameTitle:
      titleScreen();
      break;

    case GameState::GameOptions:
      optionsScreen();
      break;

    case GameState::GameCredits:
      creditsScreen();
      break;

    case GameState::GameMenu:
      menuScreen();
      break;

    case GameState::GameWork:
     break;

    case GameState::GameBuy:
     
     break;

    case GameState::GameSell:

     break;

    case GameState::GameMenu2:
     menu2Screen();
     break;
     
    case GameState::GameBus:

     break;

    case GameState::GameDice:
     diceScreen();
     break;

    case GameState::GameDiceDelay:
     diceScreenDelay();
     break;

    case GameState::GameBank:

     break;

    case GameState::GameProperties:

     break;

  }
}

constexpr uint8_t firstTitleScreenOptionIndex = 0;
constexpr uint8_t lastTitleScreenOptionIndex = 2;

constexpr uint8_t firstMenuScreenOptionIndex = 0;
constexpr uint8_t lastMenuScreenOptionIndex = 3;

constexpr uint8_t firstMenu2ScreenOptionIndex = 0;
constexpr uint8_t lastMenu2ScreenOptionIndex = 3;

constexpr Point titleScreenOptions[] { { 94, 25 }, { 83, 40 }, {83, 55} };

constexpr Point menuScreenOptions[] { { 105, 10}, {110, 25}, {106, 40}, { 105, 54} };

constexpr Point menu2ScreenOptions[] { { 110, 10}, {105, 25}, {106, 40}, {68, 54} };

void titleScreen()
{
  // If the down button is pressed
  if(arduboy.justPressed(DOWN_BUTTON))
  {
    // If the cursor index is less than the last option index
    if(cursorIndexTitle < lastTitleScreenOptionIndex)
      // Increment the cursor index
      ++cursorIndexTitle;
  }
  
  // If the up button is pressed
  if(arduboy.justPressed(UP_BUTTON))
  {
    // If the cursor index is greater than the first option index
    if(cursorIndexTitle > firstTitleScreenOptionIndex)
      // Decrement the cursor index
      --cursorIndexTitle;
  }

  // Set the cursor's position based on the current index
  cursorTitleX = (titleScreenOptions[cursorIndexTitle].x - 7);
  cursorTitleY = titleScreenOptions[cursorIndexTitle].y;

  // Draw the options
  arduboy.setCursor(titleScreenOptions[0].x, titleScreenOptions[0].y);
  arduboy.print(F("Start"));

  arduboy.setCursor(titleScreenOptions[1].x, titleScreenOptions[1].y);
  arduboy.print(F("Options"));

  arduboy.setCursor(titleScreenOptions[2].x, titleScreenOptions[2].y);
  arduboy.print(F("Credits"));

  // Draw the cursor
  drawCursorTitle();

  Sprites::drawOverwrite(12, 10, titleimg, 0);

  arduboy.setCursor(90, 1);
  arduboy.print(F("Money"));
  arduboy.setCursor(92, 12);
  arduboy.print(money);

  if (arduboy.justPressed(A_BUTTON) && cursorIndexTitle == 1) {
    gameState = GameState::GameOptions;
  }

  soundEnter();

  if (arduboy.justPressed(A_BUTTON) && cursorIndexTitle == 0) {
    gameState = GameState::GameMenu;

  }

  soundEnter();

  if (arduboy.justPressed(A_BUTTON) && cursorIndexTitle == 2) {
    gameState = GameState::GameCredits;
  }

  soundEnter();

}

void drawCursorMenu() {
  Sprites::drawOverwrite(cursorMenuX, cursorMenuY, cursor, 0);
}

void drawCursorMenu2() {
  Sprites::drawOverwrite(cursorMenu2X, cursorMenu2Y, cursor, 0);
}

void drawCursorTitle()
{
  Sprites::drawOverwrite(cursorTitleX, cursorTitleY, cursor, 0);
}

void menuScreen() {
  if (arduboy.justPressed(DOWN_BUTTON)) {
    if (cursorIndexMenu < lastMenuScreenOptionIndex)

     ++cursorIndexMenu;
  }

  if (arduboy.justPressed(UP_BUTTON)) {
    if (cursorIndexMenu > firstMenuScreenOptionIndex)

     --cursorIndexMenu;
  }

  cursorMenuX = (menuScreenOptions[cursorIndexMenu].x - 7);
  cursorMenuY = menuScreenOptions[cursorIndexMenu].y;

  arduboy.setCursor(menuScreenOptions[0].x, menuScreenOptions[0].y);
  arduboy.print(F("Work"));

  arduboy.setCursor(menuScreenOptions[1].x, menuScreenOptions[1].y);
  arduboy.print(F("Buy"));

  arduboy.setCursor(menuScreenOptions[2].x, menuScreenOptions[2].y);
  arduboy.print(F("Sell"));

  arduboy.setCursor(menuScreenOptions[3].x, menuScreenOptions[3].y);
  arduboy.print(F("Next"));

  drawCursorMenu();

  printMoney();

  if (arduboy.justPressed(B_BUTTON)) {
    gameState = GameState::GameTitle;
  }

  soundExit();

  if (arduboy.justPressed(A_BUTTON) && cursorIndexMenu == 3) {
    gameState = GameState::GameMenu2;
  }

  soundEnter();
}

void menu2Screen() {
  if (arduboy.justPressed(DOWN_BUTTON)) {
    if (cursorIndexMenu2 < lastMenu2ScreenOptionIndex)

     ++cursorIndexMenu2;
  }

  if (arduboy.justPressed(UP_BUTTON)) {
    if (cursorIndexMenu2 > firstMenu2ScreenOptionIndex)
     
     --cursorIndexMenu2;
  }

  cursorMenu2X = (menu2ScreenOptions[cursorIndexMenu2].x - 7);
  cursorMenu2Y = menu2ScreenOptions[cursorIndexMenu2].y;

  arduboy.setCursor(menu2ScreenOptions[0].x, menu2ScreenOptions[0].y);
  arduboy.print(F("Bus"));

  arduboy.setCursor(menu2ScreenOptions[1].x, menu2ScreenOptions[1].y);
  arduboy.print(F("Dice"));

  arduboy.setCursor(menu2ScreenOptions[2].x, menu2ScreenOptions[2].y);
  arduboy.print(F("Bank"));

  arduboy.setCursor(menu2ScreenOptions[3].x, menu2ScreenOptions[3].y);
  arduboy.print(F("Properties"));

  drawCursorMenu2();

  printMoney();

  if (arduboy.justPressed(B_BUTTON)) {
    gameState = GameState::GameMenu;
  }

  if (arduboy.justPressed(A_BUTTON) && cursorIndexMenu2 == 1) {
    gameState = GameState::GameDice;
  }

  soundExit();
  soundEnter();
}

void optionsScreen() {
  arduboy.setCursor(0, 0);
  arduboy.print(F("Press B to Toggle Sfx"));
  arduboy.print(F("\n"));
  arduboy.print(F("Press A to Save"));
  arduboy.print(F("\n"));
  arduboy.print(F("Press A + B to Clear "));
  arduboy.print(F("\n"));
  arduboy.print(F("Save Data"));
  arduboy.print(F("\n"));
  arduboy.print(F("Press UP + DOWN to "));
  arduboy.print(F("\n"));
  arduboy.print(F("Exit Options"));
  
  if (arduboy.justPressed(B_BUTTON)) {
    sfxToggle = !sfxToggle;
  }

  if (arduboy.justPressed(UP_BUTTON) && DOWN_BUTTON) {
    gameState = GameState::GameTitle;
  }

  soundExit();
}

void printMoney() {
  arduboy.setCursor(0, 0);
  arduboy.print(F("Money"));
  arduboy.setCursor(0, 8);
  arduboy.print(money);
}

void creditsScreen() {
  arduboy.setCursor(0, 0);
  arduboy.print(F("Ard_Flamingo"));
  arduboy.print(F("\n\n"));
  arduboy.print(F("Pharap"));

  if (arduboy.justPressed(B_BUTTON)) {
    gameState = GameState::GameTitle;
  }
    soundExit();
}

void soundEnter() {
  if (arduboy.justPressed(A_BUTTON) && sfxToggle == true) {
    sound.tone(650, 100);
  }
}

void soundExit() {
  if (arduboy.justPressed(B_BUTTON) && sfxToggle == true) {
    sound.tone(550, 100);
  }
}

void diceScreen() {

 if (arduboy.justPressed(A_BUTTON) && didDiceRoll == 0 && diceRestart == false) {
  diceRandom();
  delayDice = (millis() + (4 * 1000));
  didDiceRoll = 1;
  diceRestart == true;
 }

 if (dice < 4 && dice > 0) {
  money = money / 2;
 }

 if (dice > 3 && dice < 7) {
  money = money * 2;
 }

 if (didDiceRoll == 1) {
  gameState = GameState::GameDiceDelay;
 }

  printMoney();
  
}

void diceScreenDelay() {

  uint32_t currentDice = millis();

  if (currentDice >= delayDice) {
    didDiceRoll = 0;
    gameState = GameState::GameDice;
    diceRandom();
  }

  int remainingSecondsDice = ((delayDice - currentDice) / 1000);

  arduboy.setCursor(-1, 56);
  arduboy.print("5 Mins Until Next Roll");

  printMoney();
}

void diceRandom() {
  if (diceRestart == false) {
    dice = random(1, 7);
  }

  if (diceRestart == true) {
    dice = random(1, 7);
  }
}
