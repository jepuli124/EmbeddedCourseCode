#define F_CPU 16000000UL
#define NOTE_C5 523
#define NOTE_A7 3520
#define NOTE_B2 123

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
// You will need to include the following library: <avr/interrupt.h>.
#include <avr/interrupt.h>

#include "lcd.h"



const int DATA_OUTPUT = PD9;
const int PRESCALER64 = 64;
const int PRESCALER1 = 1;
int COUNTER = 0; //counts interupts ?

int count(char* p);

uint16_t counter = 0;
// When the ADC is complete, print the result in the LCD.
// You can save the conversion results in the ISR(interrupt_vector) sub-routine 
// ISR for timer/counter1 overflow
ISR(interrupt_vector) {
  // Reset the timer/counter1 to maintain the PWM frequency
  TCNT1 = 0;
}


int main(void) { 
 
    //Empty values
    TCNT1 = 0;
    TCCR1B = 0;

    OC1A = 0; 

    //DDRx input !TODO check that is correct port setting
    DDRD |= (1 << DATA_OUTPUT);

    // Set COM1A0 mode to compare match with OC1A
    // Set waveform generation mode to PWM, phase and frequency correct and TOP as OCR1A with WGM13 WGM10
    TCCR1A = (1 << COM1A0) | (1 << WGM13) | (1 << WGM10);
    
    // Enable the PWM output by setting the clock source (prescaler 1) should be same as constant PRESCALER1.
    // !TODO tips said to add WGM13 and WGM10 as ti actiwave mode 9.
    TCCR1B =  (1 << CS10) | (1 << WGM13) | (1 << WGM10);
    
    // Enable global timer/counters interrupts
    TIMSK1 = (1 << TOIE1);

    sei();

    

    while(1){

    TCCR1B =  (1 << CS10) | (1 << WGM13) | (1 << WGM10);
    // Note set
    OCR1A = F_CPU/(2*PRESCALER1*NOTE_C5);
    

    delay(1000); // Play the note for 1 second

    // Note set
    OCR1A = F_CPU/(2*PRESCALER1*NOTE_A7);
    

    delay(1000); // Play the note for 1 second

    // Enable the PWM output by setting the clock source (prescaler 64) should be same as constant PRESCALER64.
    TCCR1B = (1 << CS11) | (1 << CS10) | (1 << WGM13) | (1 << WGM10);
    OCR1A = F_CPU/(2*PRESCALER64*NOTE_B2);

    }
    
    return 0;
}

