#include <Timer.h>
#include <LiquidCrystal.h>

#define BUTTON_PIN 13
#define LED_PIN 12

#define TIME_MIN 1000
#define TIME_MAX 2000

#define START 0
#define WAIT 1
#define MEASURE 2
#define CHEATER 3

short state;
Timer timer = Timer();
uint32_t time_chosen;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPress, RISING);
  state = START;
}

void loop() {
  switch(state) {
    case START:
      digitalWrite(LED_PIN, 0);
    case WAIT:
      if (timer.read() > time_chosen) {
        digitalWrite(LED_PIN, 1);
      }
      else {
        digitalWrite(LED_PIN, 0);
      }
    case MEASURE:
      // show value of timer.read()
      digitalWrite(LED_PIN, 0);
    case CHEATER:
      // indicate cheating
      digitalWrite(LED_PIN, 0);
  }
}

void buttonPress() {
  switch(state) {
    case START:
      time_chosen = random(TIME_MIN, TIME_MAX);
      state = WAIT;
      timer.start();
    case WAIT:
      timer.stop();
      if (timer.read() > time_chosen){
        state = MEASURE;
      }
      else {
        state = CHEATER;
      }
    case MEASURE:
      state = START;
    case CHEATER:
      state = START;
  }
}



