/// -------- ///
/// beta 1.0 ///
/// -------- ///

/// --------
// ボタン定義
// mcp0(0x20) = ボタン0~4(5コ)
// mcp1(0x21) = ボタン5~9(5コ)
// mcp2(0x22) = ボタン10~14(5コ)

#include "Adafruit_MCP23X08.h"
#include "Adafruit_MCP23X17.h"
#include "Adafruit_MCP23XXX.h"
#include <Wire.h>
#define startTime 60000
#define MAX_BUTTON 15
#define MAX_WIRE 30
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <LiquidCrystal.h>

Adafruit_MCP23X17 mcp0;
Adafruit_MCP23X17 mcp1;
Adafruit_MCP23X17 mcp2;
void clearLight();
void gameStart();
void initGame();
LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
int isPushed(int ButtonNum, int lightTime);
// ButtonNumは0~14
// lightTimeは時間 msでもないので微調整が必要
// mcpidは使用するid -> <<使わんので削ること>>
void initPins();
int INPUT_PINS[15];
int OUTPUT_PINS[15];
int EXTEND_PINS[5];
void printLCD(int line, char *text);

int isPushed(int ButtonNum, int lightTime) {
  int flag = 0;
  bool flagBreak = false;
  for (int j = 0; j < lightTime; j++) {
    switch (ButtonNum / 5) {
      case 0:
        mcp0.digitalWrite(OUTPUT_PINS[ButtonNum % 5], HIGH);
        break;
      case 1:
        mcp1.digitalWrite(OUTPUT_PINS[ButtonNum % 5], HIGH);
        break;
      case 2:
        mcp2.digitalWrite(OUTPUT_PINS[ButtonNum % 5], HIGH);
        break;
      default:
        ;
    }
    for (int i = 0; i < MAX_BUTTON / 3; i++) {

      //      /*Debug*/ Serial.print("mcp0 Status: ");
      //      /*******/ Serial.print(INPUT_PINS[i]);
      //      /*     */ Serial.println(mcp0.digitalRead(INPUT_PINS[i]) == HIGH);
      //      /*Debug*/ Serial.print("mcp1 Status: ");
      //      /*******/ Serial.print(INPUT_PINS[i]);
      //      /*     */ Serial.println(mcp1.digitalRead(INPUT_PINS[i]) == HIGH);
      //      /*Debug*/ Serial.print("mcp2 Status: ");
      //      /*******/ Serial.print(INPUT_PINS[i]);
      //      /*     */ Serial.println(mcp2.digitalRead(INPUT_PINS[i]) == HIGH);

      if (mcp0.digitalRead(INPUT_PINS[i]) == LOW || mcp1.digitalRead(INPUT_PINS[i]) == LOW || mcp2.digitalRead(INPUT_PINS[i]) == LOW) { // 何かしらのボタンが押された
        //        /*Debug*/ Serial.println("AnyButtonispushed!");
        if (mcp0.digitalRead(INPUT_PINS[i] == LOW)) {
          if (i == ButtonNum) {
            flag = 1;
          }
          else {
            flag = -1;
          }
          flagBreak = true;
        }

        else if (mcp1.digitalRead(INPUT_PINS[i] == LOW)) {
          if (i + 5 == ButtonNum) {
            flag = 1;
          }
          else {
            flag = -1;
          }
          flagBreak = true;
        }
        else if (mcp2.digitalRead(INPUT_PINS[i] == LOW)) {
          if (i + 10 == ButtonNum) {
            flag = 1;
          }
          else {
            flag = -1;
          }
          flagBreak = true;
        }
        /*if (i == ButtonNum) { // iが指定されたボタン番号なら
          flag = 1; // Score:+1
          flagBreak = true;
          }*/
        else {
          flag = -1; // Score:-1
          flagBreak = true; // 出る
        }
      }
    }

    //    Serial.print("The flag is ");
    //    Serial.println(flagBreak);
    if (flagBreak == true) {
      break;
    }

    delay(1);
  }
  clearLight();
  // if (digitalRead(6) == LOW) flag = true;
  return flag;
}

void setup() {
  mcp0.begin_I2C(0x20);
  mcp1.begin_I2C(0x21);
  mcp2.begin_I2C(0x22);
  initPins();
  clearLight();
  //  Serial.begin(230400);
  lcd.begin(16, 2);
}

void loop() {
  delay(500);
  lcd.clear();
  bool flag = true;
  srand((unsigned int)time(NULL));
  while (flag) {
    lcd.setCursor(0, 0);
    lcd.print("Press Any Button");

    for (int i = 0; i < MAX_BUTTON / 3; i++) {
      if (mcp0.digitalRead(INPUT_PINS[i]) == LOW || mcp1.digitalRead(INPUT_PINS[i]) == LOW || mcp2.digitalRead(INPUT_PINS[i]) == LOW) {
        flag = false;
        gameStart();
      }
    }
  }
}

void initPins() {
  for (int i = 0; i < MAX_WIRE / 3; i++) {
    if (i % 2 == 0) {
      mcp0.pinMode(i, INPUT);
      mcp1.pinMode(i, INPUT);
      mcp2.pinMode(i, INPUT);
      // pinMode(i, INPUT);
      INPUT_PINS[i / 2] = i;
    }
    else {
      mcp0.pinMode(i, OUTPUT);
      mcp1.pinMode(i, OUTPUT);
      mcp2.pinMode(i, OUTPUT);
      // pinMode(i, OUTPUT);
      OUTPUT_PINS[i / 2] = i;
    }
  }
}

void gameStart() {
  lcd.clear();
  long long timer = 60000;
  int score = 0;
  int state;
  bool outflag = true;

  for (int i = 3; i > 0; i--) {
    lcd.setCursor(7, 0);
    lcd.print(i);
    delay(1000);
  }

  startLCD();
  printTime(timer / 1000);
  printScore(score);
  while (true) {
    int randtime = randint(75, 50);
    state = isPushed(randint(0, 15), randtime);
    if (state != 0) {
      score += state;
      printScore(score);
      //      Serial.println(score);
      // digitalWrite(7, LOW);
    }
    timer -= randtime * 3;
    printTime(timer / 1000.0);
    // else digitalWrite(7, HIGH);
    // digitalWrite(7, LOW);
    int waittime = 1000 - (randtime * 3.5);
    delay(waittime);
    timer -= waittime;
    printTime(timer / 1000.0);
//    /* Debug */ score = -53;
//    /* Debug */ timer = -1.0;
    if (timer <= 0) {
      break;
    }
  }
  clearLight();
  lcd.clear();
  lcd.print("Score: 0");
  if (score >= 0) {
    for (double i = 0.0; i <= score; i++) {
      printScore(i);
      delay(500 * ((i / score) * (i / score) * (i / score) * (i / score)));
    }
  }
  else {
    for (double i = 0.0; i >= score; i--) {
      printScore(i);
      delay(500 * (abs(abs(i) / abs(score)) * abs(abs(i) / abs(score)) * abs(abs(i) / abs(score)) * abs(abs(i) / abs(score))));
    }
  }
  delay(1400);
  lcd.setCursor(0, 1);
  lcd.print("Thanks playing:D");
  while (outflag) {
    for (int i = 0; i < MAX_BUTTON / 3; i++) {
      if (mcp0.digitalRead(INPUT_PINS[i]) == LOW || mcp1.digitalRead(INPUT_PINS[i]) == LOW || mcp2.digitalRead(INPUT_PINS[i]) == LOW) {
        outflag = false;
      }
    }
  }
}

int randint(int adjust, int range) {
  return adjust + rand() % range;
}

void startLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Score: ");
  lcd.setCursor(0, 1);
  lcd.print("Remain: ");
}

void printTime(double remain) {
  lcd.setCursor(8, 1);
  lcd.print(remain);
}

void clearLight(){
  for (int i = 0; i < 5; i++){
    mcp0.digitalWrite(OUTPUT_PINS[i], LOW);
    mcp1.digitalWrite(OUTPUT_PINS[i], LOW);
    mcp2.digitalWrite(OUTPUT_PINS[i], LOW);
  }
}

void printScore(int score) {
  lcd.setCursor(7, 0);
  lcd.print(score);
}
