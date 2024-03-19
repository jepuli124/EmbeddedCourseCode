#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include <avr/interrupt.h>

#include "lcd.h"

const int LED_BUILTIN = PB7;
const int THERMISTOR = PK2;

const int LCD_RS = PH6;
const int LCD_RW = PB4;
const int LCD_EN = PB5;

const int LCD_D4 = PE5;
const int LCD_D5 = PG5;
const int LCD_D6 = PE3;
const int LCD_D7 = PH3;

volatile uint16_t g_adc_voltage = 0;
volatile short g_b_is_adc_complete = 0;


/* When the timer/counter matches, the ADC starts */

ISR(TIMER0_COMPA_vect) {
}   
ISR (ADC_vect)
{
    g_adc_voltage = ADC;
    g_b_is_adc_complete = 1;
    // PORTB &= ~(1 << LED_BUILTIN);
}


int main(void) {
    /* set up the ports and pins */
    DDRK &= ~(1 << THERMISTOR); //A10 as an input
    
    // DDRB |= (1 << LED_BUILTIN);
    // Set as input
    DDRB |= (1 << LCD_RW) | (1 << LCD_EN); 
    DDRE |= (1 << LCD_D4) | (1 << LCD_D6); 
    DDRG |= (1 << LCD_D5);
    DDRH |= (1 << LCD_RS) | (1 << LCD_D7); 
    
    
     /* initialize display, cursor off */
    lcd_init(LCD_DISP_ON);
        
    /* clear display */
    lcd_clrscr();
    /* print string */
    lcd_puts("The ADC result:");
    lcd_gotoxy(0,1);
    
    
    // Enable interrupts
    
    /* set up the 8-bit timer/counter0 */
    TCNT0  = 0; // reset timer/counter0 register 
    TCCR0A = 0x02; // CTC, top is OCR0A
    TCCR0B = 0b00000101; // 1024 prescaling
    // TCCR0B |= 0x06;
    OCR0A = 0xFF; // set value to which to count /compare 
    // OCR0A = 0b11111111;
    // OCR0A = 255;
    TIMSK0 |= (1 << 1); // enable compare match A interrupt
        /* Set up the ADC conversion */
    //ADMUX  |= 0b01000000; // AV_cc as voltage reference
    // ADMUX  |= (3 << 6); 
    // ADMUX  |= C0;

    ADMUX  = 0x4A; // A10 input channel
    ADCSRA = 0xAF;
    ADCSRB = 0x03;
    // ADCSRA |= (1 << 7); // turn ADC ON 
    // // ADCSRA |= 0b10000000;
    // ADCSRA |= (1 << 5); // enable ADC auto trigger
    // // ADCSRA |= 0b00100000;
    // ADCSRA |= (1 << 3); // enable ADC interrupt
    // // ADCSRA |= 0b00001000;
    // ADCSRA |= 5; // prescaler for the ADC, now 32
    // // ADCSRA |= 0b00000101;
    // ADCSRB |= 3; // timer/counter0 compare match A triggers ADC 
    // ADCSRB |= 0b00000011;

    // ADMUX  |= 0b01011010; // AVCC, 
    // ADCSRA |= 0b10101111; // EN, ADATE, IEN, prescaler 128
    // ADCSRB |= 0b00000011; // TCNT0 Compare A
    // PORTB &= ~(1 << LED_BUILTIN);
    // PORTB |= (1 << LED_BUILTIN);
    
    sei();
   
    while (1) 
    {
        /* When the ADC is complete, read the data and print it to serial */
        if (g_b_is_adc_complete)
        {
            g_b_is_adc_complete = 0;
            char adc_voltage_string[16]; // 16-bit array, assumes that the int given is 16-bits
            itoa(g_adc_voltage, adc_voltage_string, 10);
            /* clear display */
            lcd_clrscr();
            /* print ADC result string */
            lcd_puts("The ADC result:");
            lcd_gotoxy(0,1);
            //lcd_puts(adc_voltage_string);
            lcd_puts(adc_voltage_string);
            // PORTB &= ~(1 << LED_BUILTIN);

        }
        _delay_ms(250);
        
    }

    return 0;
}


  // TIMSK0 |= 2;
    // TIMSK0 |= 0x02;
    // TIMSK0 |= 0b00000010;
    

        /* The final register values for ADC
    ADMUX  = 0b01000011;
    ADCSRA = 0b10101101; // ADIE, ADEN, ADATE prescaler 32
    ADCSRB = 0b00000011; // timer/counter0 compare match A 
    */  