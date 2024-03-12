#include <avr/sleep.h>
#include <LiquidCrystal.h>

// Initialize the LCD library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

volatile int interruptCounter = 0;
unsigned long timeCounter = 0;
unsigned long previousMillis = 0;

void setup() {
  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  pinMode(2, INPUT_PULLUP); // Button for sleep
  pinMode(7, INPUT_PULLUP); // Button for wake up

  // Set INT0 to trigger on falling edge
  EICRA |= (1 << ISC01);
  EICRA &= ~(1 << ISC00);

  // Enable INT0
  EIMSK |= (1 << INT0);
}

void loop() {
  if (digitalRead(2) == LOW) {
    sleepNow();
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    if (digitalRead(7) == HIGH) {
      timeCounter++;
      lcd.setCursor(0, 1);
      lcd.print("Time: ");
      lcd.print(timeCounter);
    }
  }
}

void sleepNow() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
}

ISR(INT0_vect) {
  interruptCounter++;
  lcd.setCursor(0, 0);
  lcd.print("Interrupts: ");
  lcd.print(interruptCounter);
}