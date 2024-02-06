#include <Timer.h>
#include <LiquidCrystal.h>

#define BUTTON_PIN 2
#define LED_PIN 33

#define TIME_MIN 1000
#define TIME_MAX 2000

#define START 0
#define WAIT 1
#define MEASURE 2
#define CHEATER 3

short state;
Timer timer = Timer();
uint32_t time_chosen;

const int rs = 53, en = 50, d0 = 49, d1 = 46, d2 = 45, d3 = 42, d4 = 41, d5 = 38, d6 = 37, d7 = 34;
LiquidCrystal lcd = LiquidCrystal(rs, en, d0, d1, d2, d3, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  Serial.println("lolxd");
  pinMode(BUTTON_PIN, INPUT);
  lcd.begin(16,2);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPress, RISING);
  lcd.clear();
  lcd.print("START");
  state = START;
}

void loop() {
  switch(state) {
    case START:
      digitalWrite(LED_PIN, 0);
      break;
    case WAIT:
      if (timer.read() > time_chosen) {
        digitalWrite(LED_PIN, 1);
      }
      else {
        digitalWrite(LED_PIN, 0);
      }
      break;
    case MEASURE:
      digitalWrite(LED_PIN, 0);
      break;
    case CHEATER:
      digitalWrite(LED_PIN, 0);
      break;
  }
}

void buttonPress() {
  Serial.println("painettiin");
  switch(state) {
    case START:
      time_chosen = random(TIME_MIN, TIME_MAX);
      lcd.clear();
      lcd.print("WAIT");
      state = WAIT;
      timer.start();
      break;
    case WAIT:
      timer.stop();
      if (timer.read() > time_chosen){
        lcd.clear();
        lcd.print(timer.read()-time_chosen);
        state = MEASURE;
      }
      else {
        lcd.clear();
        lcd.print("CHEATER");
        state = CHEATER;
      }
      break;
    case MEASURE:
      lcd.clear();
      lcd.print("START");
      state = START;
      break;
    case CHEATER:
      lcd.clear();
      lcd.print("START");
      state = START;
      break;
  }
}


