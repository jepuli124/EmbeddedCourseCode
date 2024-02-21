#define F_CPU 16000000UL

#define NOTE_C5 523
#define NOTE_A7 3520
#define NOTE_B2 123
#define LED_BLINK 10

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
// You will need to include the following library: <avr/interrupt.h>.
#include <avr/interrupt.h>

// Connect a passive buzzer to digital pin 9 of the UNO and set the DDRx as output.
const int BUZZER = PB1;
const int BUILTIN = PB5;

const int PRESCALER_64 = 64;
const int PRESCALER_1 = 1;

volatile int cnt = 0;

ISR(TIMER1_COMPA_vect) {
  // Reset the timer/counter1 to maintain the PWM frequency
  TCNT1 = 0;
  // if (cnt >= 50) {
  PORTB ^= (1 << BUILTIN);  
  // }
  // cnt = (cnt >= 50) ? cnt = 0 : ++cnt;
  // Debug led toggle
  
}

int main(void) {

  // Zero registers
  TCCR1B = 0;
  OCR1A = 0;

  // Buzzer and builtin led output
  DDRB |= (1 << BUZZER);
  DDRB |= (1 << BUILTIN);


  // Set COM1A0 mode to compare match with OC1A
  // Set waveform generation mode to PWM, phase and frequency correct and TOP as
  // OCR1A with WGM13 WGM10
  //TCCR1A = (1 << COM1A0) | (1 << WGM10);

  TCCR1A = (1 << COM1A0); // COM1A0
  TCCR1A = (1 << WGM10); // WGM10

  // TCCR1B = (1 << 4); // WGM13
  // Prescaler 1
  TCCR1B = (1 << CS10) | (1 << WGM13);

  // Enable COMPA interrupt
  TIMSK1 = (1 << OCIE1A);


  // Toggling builtin led works, lower note -> lower light output,
  //     higher note -> higher light output
  // Sets the 
  TCNT1 = 0;
  
  //OCR1A = F_CPU / (2 * PRESCALER_1 * LED_BLINK);
  
  // TCCR1B = (1 << CS10) | (1 << CS11) | (1 << WGM13) | (1 << WGM10); // Prescale 64
  // OCR1A = F_CPU / (2 * PRESCALER_64 * NOTE_B2);

  sei();

  while (1) {
    // Reset to 0 and set prescaler 1
    TCCR1B = 0;
    TCCR1B = (1 << CS10) | (1 << WGM13);
    
    OCR1A = F_CPU / (2 * PRESCALER_1 * NOTE_C5);

    _delay_ms(500);

    OCR1A = F_CPU / (2 * PRESCALER_1 * NOTE_A7);

    _delay_ms(750);

    // Enable the PWM output by setting the clock source (prescaler 64) should
    // be same as constant PRESCALER64.
    TCCR1B = (1 << CS11) | (1 << CS10);
    OCR1A = F_CPU / (2 * PRESCALER_64 * NOTE_B2);
    
    _delay_ms(500);

  }

  return 0;
}


    // // Note set
    // OCR1A = F_CPU / (2 * PRESCALER1 * NOTE_A7);
    // Enable the PWM output by setting the clock source (prescaler 64) should
    // be same as constant PRESCALER64.
    // TCCR1B = (1 << CS11) | (1 << CS10) | (1 << WGM13) | (1 << WGM10);
    // OCR1A = F_CPU / (2 * PRESCALER_64 * NOTE_B2);