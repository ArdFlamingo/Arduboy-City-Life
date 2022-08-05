#include <ArduboyTonesPitches.h>
#include <ArduboyTones.h>
#include <Arduboy2.h>
#include <EEPROM.h>
#include "sprites.h"
#include "tones.h"
#include "src/fonts/Font4x6.h"

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);
Font4x6 font4x6 = Font4x6();

uint8_t menuItemsTitle = 3;
uint8_t menuItemsMenu = 4;
uint8_t menuItemsMenu2 = 4;
uint8_t GameStart = 1;
bool sfxToggle = true;

uint8_t dice = 0;

uint8_t cursorIndexTitle = 0;
uint8_t cursorTitleX = 88;
uint8_t cursorTitleY = 25;

uint8_t cursorIndexMenu = 0;
uint8_t cursorMenuX = 88;
uint8_t cursorMenuY = 25;

uint8_t cursorIndexMenu2 = 0;
uint8_t cursorMenu2X = 88;
uint8_t cursorMenu2Y = 25;

uint32_t propertyTimer = 0;

uint8_t ticketUse = 0;
uint8_t didTicketWin = false;
uint32_t lotteryTimer = 0;

constexpr Point foodOptions[] { {2, 24}, {30, 22}, {50, 8}, {66, 35}, {95, 45}, {110, 5}, { 15, 45}, {80, 15}, {107, 28} };

uint8_t playerX = 40;
uint8_t playerY = 45;
uint8_t customerOrder = 0;




  Rect FriedChicken (foodOptions[0].x, foodOptions[0].y, 8, 8);
  Rect Donut (foodOptions[1].x, foodOptions[1].y, 8, 8);
  Rect Burger (foodOptions[2].x, foodOptions[2].y, 8, 8);
  Rect Milk (foodOptions[3].x, foodOptions[3].y, 8, 8);
  Rect Hotdog (foodOptions[4].x, foodOptions[4].y, 8, 8);
  Rect Soda (foodOptions[5].x, foodOptions[5].y, 8, 8);
  Rect Fries (foodOptions[6].x, foodOptions[6].y, 8, 8);
  Rect Pizza (foodOptions[7].x, foodOptions[7].y, 8, 8);
  Rect Juice (foodOptions[8].x, foodOptions[8].y, 8, 8);

enum class GameState : uint8_t {
  GameTitle,
  GameOptions,
  GameCredits,
  GameStart,
  GameMenu,
  GameWork,
  GameBuy,
  GamePhone,
  GameMenu2,
  GameDice,
  GameBank,
  GameProperties,
  GamePropertiesBought,
};

struct SaveData {
  uint32_t money = 80000;
  uint32_t moneyBank = 0;
  bool isPropertyBought = false;
  uint8_t ticketsOwned = 0;
};

SaveData saveData;

#define EEPROM_STORAGE_SPACE_START_ARDUBOY EEPROM_STORAGE_SPACE_START + 700

GameState gameState = GameState::GameTitle;

void setup()
{
  arduboy.begin();
  arduboy.initRandomSeed();
  arduboy.setFrameRate(55);
  customerOrder = random(1, 10);
  ticketUse = random(1, 26);

  EEPROM.get(EEPROM_STORAGE_SPACE_START_ARDUBOY, saveData);
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
     workScreen();
     break;

    case GameState::GameBuy:
     buyScreen();
     break;

    case GameState::GamePhone:
     phoneScreen();
     break;

    case GameState::GameMenu2:
     menu2Screen();
     break;

    case GameState::GameDice:
     diceScreen();
     break;

    case GameState::GameBank:
     bankScreen();
     break;

    case GameState::GameProperties:
     propertiesScreen();
     break;

    case GameState::GamePropertiesBought:
     propertiesBoughtScreen();
     break;
  }
}

constexpr uint8_t firstTitleScreenOptionIndex = 0;
constexpr uint8_t lastTitleScreenOptionIndex = 2;

constexpr uint8_t firstMenuScreenOptionIndex = 0;
constexpr uint8_t lastMenuScreenOptionIndex = 3;

constexpr uint8_t firstMenu2ScreenOptionIndex = 0;
constexpr uint8_t lastMenu2ScreenOptionIndex = 2;

constexpr Point titleScreenOptions[] { { 94, 25 }, { 83, 40 }, {83, 55} };

constexpr Point menuScreenOptions[] { { 105, 10}, {110, 25}, {98, 40}, { 105, 54} };

constexpr Point menu2ScreenOptions[] { {105, 25}, {106, 40}, {68, 54} };

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
  arduboy.print(saveData.money);

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

  startTimerCounting();

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
  arduboy.print(F("Phone"));

  arduboy.setCursor(menuScreenOptions[3].x, menuScreenOptions[3].y);
  arduboy.print(F("Next"));

  drawCursorMenu();

  printMoney();

  if (arduboy.justPressed(B_BUTTON)) {
    gameState = GameState::GameTitle;
  }
  if (arduboy.justPressed(A_BUTTON) && cursorIndexMenu == 0) {
    gameState = GameState::GameWork;
  }

  if (arduboy.justPressed(A_BUTTON) && cursorIndexMenu == 2) {
    gameState = GameState::GamePhone;
  }

  if (arduboy.justPressed(A_BUTTON) && cursorIndexMenu == 1) {
    gameState = GameState::GameBuy;
  }
  

  soundExit();

  if (arduboy.justPressed(A_BUTTON) && cursorIndexMenu == 3) {
    gameState = GameState::GameMenu2;
  }

  startTimerCounting();

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
  arduboy.print(F("Dice"));

  arduboy.setCursor(menu2ScreenOptions[1].x, menu2ScreenOptions[1].y);
  arduboy.print(F("Bank"));

  arduboy.setCursor(menu2ScreenOptions[2].x, menu2ScreenOptions[2].y);
  arduboy.print(F("Properties"));

  drawCursorMenu2();

  printMoney();

  if (arduboy.justPressed(B_BUTTON)) {
    gameState = GameState::GameMenu;
  }

  if (arduboy.justPressed(A_BUTTON) && cursorIndexMenu2 == 0) {
    gameState = GameState::GameDice;
  }

  if (arduboy.justPressed(A_BUTTON) && cursorIndexMenu2 == 1) {
    gameState = GameState::GameBank;
  }

  if (arduboy.justPressed(A_BUTTON) && cursorIndexMenu2 == 2 && saveData.isPropertyBought == false) {
    gameState = GameState::GameProperties;
  }

  if (arduboy.justPressed(A_BUTTON) && cursorIndexMenu2 == 2 && saveData.isPropertyBought == true) {
    gameState = GameState::GamePropertiesBought;
  }

  startTimerCounting();

  soundExit();
  soundEnter();
}

void optionsScreen() {
  arduboy.setCursor(0, 0);
  arduboy.print(F("Press B to Toggle Sfx"));
  arduboy.print(F("\n"));
  arduboy.print(F("Press A to Save"));
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

  if (arduboy.justPressed(A_BUTTON)) {
    EEPROM.put(EEPROM_STORAGE_SPACE_START_ARDUBOY, saveData);
  }

  startTimerCounting();

  soundExit();
}

void printMoney() {
  arduboy.setCursor(0, 0);
  arduboy.print(F("Money"));
  arduboy.setCursor(0, 8);
  arduboy.print(saveData.money);
}

void creditsScreen() {
  arduboy.setCursor(0, 0);
  arduboy.print(F("Ard_Flamingo"));
  arduboy.print(F("\n\n"));
  arduboy.print(F("Pharap"));
  arduboy.print(F("\n\n"));
  arduboy.print(F("filmote"));

  startTimerCounting();

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
  
  if (arduboy.justPressed(A_BUTTON)) {
  if (saveData.money >= 100) {
  dice = random(1, 7);

 if (dice == 1 || dice == 2 || dice == 3 || dice == 4) {
  saveData.money = saveData.money / 2;
 }

 if (dice == 5 || dice == 6) {
  saveData.money = saveData.money * 2;
 }
 
}

}

startTimerCounting();

printMoney();
drawDiceImg();

arduboy.setCursor(0, 54);
arduboy.print(F("You Rolled "));
arduboy.print(dice);

if (arduboy.justPressed(B_BUTTON)) {
  gameState = GameState::GameMenu2;
  soundExit();
}

}

void drawDiceImg() {
  Sprites::drawOverwrite(80, 5, diceimg, 0);
}

void workScreen() {

  if (arduboy.pressed(UP_BUTTON)) {
    playerY -= 1;
  }

  if (arduboy.pressed(DOWN_BUTTON)) {
    playerY += 1;
  }

  if (arduboy.pressed(RIGHT_BUTTON)) {
    playerX += 1;
  }

  if (arduboy.pressed(LEFT_BUTTON)) {
    playerX -= 1;
  }

 Rect playerRect = getPlayerRect();
 

 if (arduboy.collide(playerRect, FriedChicken) && customerOrder == 1) {
  if (arduboy.justPressed(A_BUTTON)) {
    saveData.money += 50;
    customerOrderRandom();
    soundEnter();
  }
 }

 if (arduboy.collide(playerRect, Pizza) && customerOrder == 2) {
  if (arduboy.justPressed(A_BUTTON)) {
    saveData.money += 50;
    customerOrderRandom();
    soundEnter();
  }
 }

 if (arduboy.collide(playerRect, Fries) && customerOrder == 3) {
  if (arduboy.justPressed(A_BUTTON)) {
    saveData.money += 50;
    customerOrderRandom();
    soundEnter();
  }
 }

 if (arduboy.collide(playerRect, Soda) && customerOrder == 4) {
  if (arduboy.justPressed(A_BUTTON)) {
    saveData.money += 50;
    customerOrderRandom();
    soundEnter();
  }
 }

 if (arduboy.collide(playerRect, Milk) && customerOrder == 5) {
  if (arduboy.justPressed(A_BUTTON)) {
    saveData.money += 50;
    customerOrderRandom();
    soundEnter();
  }
 }

 if (arduboy.collide(playerRect, Juice) && customerOrder == 6) {
  if (arduboy.justPressed(A_BUTTON)) {
    saveData.money += 50;
    customerOrderRandom();
    soundEnter();
  }
 }

 if (arduboy.collide(playerRect, Donut) && customerOrder == 7) {
  if (arduboy.justPressed(A_BUTTON)) {
    saveData.money += 50;
    customerOrderRandom();
    soundEnter();
  }
 }

 if (arduboy.collide(playerRect, Burger) && customerOrder == 8) {
  if (arduboy.justPressed(A_BUTTON)) {
    saveData.money += 50;
    customerOrderRandom();
    soundEnter();
  }
 }

 if (arduboy.collide(playerRect, Hotdog) && customerOrder == 9) {
  if (arduboy.justPressed(A_BUTTON)) {
    saveData.money += 50;
    customerOrderRandom();
    soundEnter();
  }
 }

 startTimerCounting();
 
 drawWorkFood();
 drawWorkPlayer();
 printMoney();

 font4x6.setCursor(0, 56);
 font4x6.print(F("Customer Orders Number:"));
 font4x6.print(customerOrder);

  

 if (arduboy.justPressed(B_BUTTON)) {
  gameState = GameState::GameMenu;
  soundExit();
 }
  
}

void drawWorkFood() {
  Sprites::drawOverwrite(foodOptions[0].x, foodOptions[0].y, friedchicken, 0);
  Sprites::drawOverwrite(foodOptions[1].x, foodOptions[1].y, donut, 0);
  Sprites::drawOverwrite(foodOptions[2].x, foodOptions[2].y, burger, 0);
  Sprites::drawOverwrite(foodOptions[3].x, foodOptions[3].y, milk, 0);
  Sprites::drawOverwrite(foodOptions[4].x, foodOptions[4].y, hotdog, 0);
  Sprites::drawOverwrite(foodOptions[5].x, foodOptions[5].y, soda, 0);
  Sprites::drawOverwrite(foodOptions[6].x, foodOptions[6].y, fries, 0);
  Sprites::drawOverwrite(foodOptions[7].x, foodOptions[7].y, pizza, 0);
  Sprites::drawOverwrite(foodOptions[8].x, foodOptions[8].y, juice, 0);
}

void drawWorkPlayer() {
  Sprites::drawOverwrite(playerX, playerY, workcursor, 0);
}

Rect getPlayerRect() {

    Rect playerRect;
    playerRect.x = playerX;
    playerRect.y = playerY;
    playerRect.width = 5;
    playerRect.height = 5;

    return playerRect;
}

void customerOrderRandom() {
  customerOrder = random (1, 10);
}

void bankScreen() {

  if (arduboy.justPressed(A_BUTTON) && saveData.money > 0) {
    saveData.money -= 50;
    saveData.moneyBank += 50;
    soundEnter();
  }

  if (arduboy.justPressed(B_BUTTON) && saveData.moneyBank > 0) {
    saveData.moneyBank -= 50;
    saveData.money += 50;
    soundEnter();
  }

  if (arduboy.justPressed(UP_BUTTON) && DOWN_BUTTON) {
    gameState = GameState::GameMenu2;
    soundExit();
  }

  
  startTimerCounting();

  printMoney();
  drawBankImg();
  arduboy.setCursor(0, 56);
  arduboy.print(F("Account Balance:"));
  arduboy.print(saveData.moneyBank);
}

void drawBankImg() {
  Sprites::drawOverwrite (80, 5, bankimg, 0);
}

void propertiesScreen() {

    printMoney();
    drawPropertiesImg(); 

    arduboy.setCursor(0, 48);
    arduboy.print(F("One Property For Sale:"));
    arduboy.print(F("\n"));
    arduboy.print(F("174 Wallyflair Avenue"));
    arduboy.setCursor(0, 35);
    arduboy.print(F("Buy $80000"));

    if (arduboy.justPressed(B_BUTTON)) {
    gameState = GameState::GameMenu2;
    soundExit();
  }   
    
    if (arduboy.justPressed(A_BUTTON) && saveData.money >= 80000) {
    saveData.isPropertyBought = true;
    saveData.money -= 80000;
    gameState = GameState::GamePropertiesBought;
    soundEnter();
    
    }

    if (arduboy.justPressed(A_BUTTON)) {
      startTimer();
    }
}

void drawPropertiesImg() {
  Sprites::drawOverwrite(80, 5, houseimg, 0);
}

void propertiesBoughtScreen() {

  printMoney();
  drawPropertiesImg();

  arduboy.setCursor(-1, 48);
  arduboy.print(F("Property Sold For Rent"));
  arduboy.print(F("\n"));
  arduboy.print(F("174 Wallyflair Avenue"));
  arduboy.setCursor(0, 35);
  arduboy.print(F("$100/3 Sec"));

  if (arduboy.justPressed(B_BUTTON)) {
    gameState = GameState::GameMenu2;
    soundExit();
  }

  startTimerCounting();
}

void startTimer() {
  propertyTimer = (millis() + (3 * 1000));
}

void startTimerCounting() {
    uint32_t propertyTimerCurrent = millis();

  if (propertyTimerCurrent >= propertyTimer && saveData.isPropertyBought == true) {
    saveData.money += 100;
    startTimer();
  }
}

void phoneScreen() {
  
  startTimerCounting();
  printMoney();
  drawPhoneImg();

  arduboy.setCursor(0, 48);
  arduboy.print(F("A = Track A"));
  arduboy.print(F("\n"));
  arduboy.print(F("B = Track B"));

  if (arduboy.justPressed(A_BUTTON)) {
    sound.tones(trackA);
  }

  if (arduboy.justPressed(B_BUTTON)) {
    sound.tones(trackB);
  }

  if (arduboy.justPressed(UP_BUTTON) && DOWN_BUTTON) {
    gameState = GameState::GameMenu;
    soundExit();
  }
}

void drawPhoneImg() {
  Sprites::drawOverwrite(80, 5, phoneimg, 0);
}

void buyScreen() {

  startTimerCounting();
  printMoney();
  drawBuyImg();

  arduboy.setCursor(0, 56);
  arduboy.print(F("Tickets Owned = "));
  arduboy.print(saveData.ticketsOwned);

  if (arduboy.justPressed(A_BUTTON) && saveData.money >= 100) {
    saveData.ticketsOwned += 1;
    saveData.money -= 100;
  }

  if (arduboy.justPressed(B_BUTTON) && saveData.ticketsOwned > 0 && ticketUse < 25) {
    saveData.ticketsOwned -= 1;
    ticketUse = random(1, 26);
  }

  if (arduboy.justPressed(B_BUTTON) && saveData.ticketsOwned > 0 && ticketUse == 25) {
    saveData.ticketsOwned -= 1;
    saveData.money += 10000;
    didTicketWin = true;
    ticketUse = random(1, 26);
    LotteryTimer();
  }

  if (didTicketWin == true) {
    arduboy.setCursor(0, 35);
    arduboy.print(F("You Won!"));
  }

  LotteryTimerCounting();

  if (arduboy.justPressed(UP_BUTTON) && DOWN_BUTTON) {
    gameState = GameState::GameMenu;
    soundExit();
  }
}

void LotteryTimer() {
  if (didTicketWin == true) {
  lotteryTimer = (millis() + 3 * 1000);
}
}

void LotteryTimerCounting() {
    uint32_t lotteryTimerCurrent = millis();

  if (lotteryTimerCurrent >= lotteryTimer) {
    didTicketWin = false;
    arduboy.setCursor(0, 35);
    arduboy.print(F("."));
  }
}

void drawBuyImg() {
  Sprites::drawOverwrite(80, 5, buyimg, 0);
}
