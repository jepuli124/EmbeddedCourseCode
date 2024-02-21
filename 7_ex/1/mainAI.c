#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>


#define NOTE_C5 523
#define NOTE_A7 3520
#define NOTE_B2 123

void setup() {
  DDRB |= (1 << DDB1); // Set DDRx as output for digital pin 9 (OC1A)
  
  // Reset timer/counter1 and TCCR1B
  TCNT1 = 0;
  TCCR1B = 0;

  // Set output mode to toggle OC1A on compare match
  // Set waveform generation mode to PWM, phase and frequency correct
  TCCR1A = (1 << COM1A0) | (1 << WGM13);
  
  // Enable timer/counters interrupts
  TIMSK1 = (1 << TOIE1);

  // Enable the microcontrollers interrupts
  sei();
}

void loop() {
  // Calculate the TOP value for notes C5 and A7
  OCR1A = F_CPU / (2 * 1 * NOTE_C5) - 1;
  
  // Enable the PWM output by setting the clock source (prescaler 1)
  TCCR1B = (1 << CS10);

  delay(1000); // Play the note for 1 second

  // Calculate the TOP value for note B2
  OCR1A = F_CPU / (2 * 64 * NOTE_B2) - 1;
  
  // Enable the PWM output by setting the clock source (prescaler 64)
  TCCR1B = (1 << CS11) | (1 << CS10);

  delay(1000); // Play the note for 1 second
}

// ISR for timer/counter1 overflow
ISR(TIMER1_OVF_vect) {
  // Reset the timer/counter1 to maintain the PWM frequency
  TCNT1 = 0;
}