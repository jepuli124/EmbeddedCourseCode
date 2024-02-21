#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
// You will need to include the following library: <avr/interrupt.h>.
#include <avr/interrupt.h>

#include "lcd.h"

const int THERMISTOR = PC3;
const int BUILTIN = PB5;

const int LCD_RS = PB1;
const int LCD_RW = PB2;
const int LCD_EN = PB3;

const int LCD_D4 = PD3;
const int LCD_D5 = PD4;
const int LCD_D6 = PD5;
const int LCD_D7 = PD7;


volatile short is_timer_Ready = 0;
volatile uint16_t adc = 0; 

// When the ADC is complete, print the result in the LCD.
// You can save the conversion results in the ISR(interrupt_vector) sub-routine 
ISR(ADC_vect)
{
    // Debugging led
    PORTB ^= (1 << BUILTIN);
    is_timer_Ready = 1;
    adc = ADC;
}


int main(void) {
    // Debug led
    DDRB |= (1 << BUILTIN);
    // OUTPUTS CONTROL
    DDRB |= (1<<LCD_RS) | (1 << LCD_RW ) | (1 << LCD_EN ); 
    // OUTPUTS DATA
    DDRD |= (1 << LCD_D4 )| (1 << LCD_D5 )| (1 << LCD_D6 )| (1 << LCD_D7 );    
    // Thermistor input 
    
    // Connect a thermistor to the analog pin A3 in the UNO and set the DDRx as input. DONE
    DDRC &= ~(1 << THERMISTOR);

    lcd_init(LCD_DISP_ON);
    lcd_clrscr();
    lcd_puts("Hello There!");
    
    /*
    TODO: 
    - Set up the 8-bit counter by accessing the timer/counter0 registers A and B (TCCR0A and TCCR0B).
    - You can try the effects of different prescalers in the TCCR0B.
    - Enable the interrupt in TIMSK0 
    */
    
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;
    
    // WGM01 -> Set the operation mode to clear timer on compare CTC.
    // COM0A0 -> Toggle OC0A on Compare Match  |  Use toggle output on match compare. 
    //TCCR0A |= (1 << WGM01) | (1 << COM0A0);
    
    TCCR0A = 0x02;

    // Set the TOP to be 255
    OCR0A = 255;
    
    // compare match on the compare register A (OC0A).
    TIMSK0 |= (1 << OCIE0A);
    
    // Set prescaling to 1024
    TCCR0B |= (1 << CS02) | (1 << CS00);


    /*
    - To set up the ADC you will need to use the ADC multiplexer selection register 
        (ADMUX) to select voltage reference and input channel.
    - Use ADC control and status register A to enable the ADC, 
    - uto triggering and the ADC interrupt.
    - The ADC control and status register B can be used 
        to set the auto trigger source (timer/counter0 compare match A).

    */


    ADMUX = 0;
    
    // REFS0 -> AV_cc | MUX1, MUX0 -> Analog Channel 3 pin A3
    ADMUX |= (1 << REFS0) | (1 << MUX1) | (1 << MUX0) ; 

    //         ADC Enable |ADC StartConv| ADC AutoTrig | ADC InterrEn||ADC PreScaler 1 0 1 -> 32 
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE) | (1 << ADIE)  | (1 << ADPS2) | (1 << ADPS0);
    
    ADCSRB = 0;
    // Timer0 compare match A | 0 1 1
    ADCSRB |= (1 << ADTS1) | (1 << ADTS0);
 
    // To enable the micro-controllers interrupts use sei().
    sei();
    
    
    // reset the timer/counter0 in the main(void) before while(1).
    TCNT0 = 0;
    
    while(1) {
        if (is_timer_Ready) {
            lcd_gotoxy(0,0);
            is_timer_Ready = 0;
            char buf[10];
            itoa(adc, buf, 10);
            
            lcd_clrscr();
            lcd_puts(buf);
            adc = 0;
        }
    }
    
    return 0;
}

