#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "lcd.h"

const int THERMISTOR = PC3;

const int LCD_RS = PB1;
const int LCD_RW = PB2;
const int LCD_EN = PB3;

const int LCD_D4 = PD3;
const int LCD_D5 = PD4;
const int LCD_D6 = PD5;
const int LCD_D7 = PD6;

int count(char* p);

int main(void) {

    // OUTPUTS CONTROL
    DDRB |= (1<<LCD_RS) | (1 << LCD_RW ) | (1 << LCD_EN ); 
    
    // OUTPUTS DATA
    DDRD |= (1 << LCD_D4 )| (1 << LCD_D5 )| (1 << LCD_D6 )| (1 << LCD_D7 );
    
    // Thermistor input 
    DDRC &= ~(1 << THERMISTOR);

    TCCR0A = 0; // Clear the register
    TCCR0B = 0;
    TCCR0A |= (1 << WGM01);
    TCNT0 = 0;
    // Set the TOP to be 255
    OCR0A = 255;

    // Set compare match on the compare register A (OC0A)
    // Use toggle output on match compare
    TCCR0A |= (1 << COM0A0);

    // Try different prescalers in the TCCR0B
    // For example, set prescaler to 64
    TCCR0B |= (1 << CS01) | (1 << CS00);

    // Enable the interrupt in TIMSK0
    TIMSK0 |= (1 << OCIE0A);

    // Set up the ADC
    ADMUX = 0; // Clear the register

    // Select voltage reference and input channel
    // For example, use AVCC as voltage reference and ADC3 as input channel
    ADMUX |= (1 << REFS0) | (1 << MUX1) | (1 << MUX0);

    // Enable the ADC, auto triggering, and the ADC interrupt
    ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADIE);

    // Set the auto trigger source (timer/counter0 compare match A)
    ADCSRB = (1 << ADTS1) | (1 << ADTS0);

    sei();
    lcd_init(LCD_DISP_ON);

    return 0;
}

int count(char* p) {
    int count = 0;
    while(p[count] != '\0') {
        count++;
    }
    return count;
}

void print_to_lcd(uint16_t result) {
    char buffer[10];
    itoa(result, buffer, 10); // Convert the result to string
    lcd_puts(buffer); // Display the result on the LCD
}

ISR(ADC_vect)
{
    // Save the conversion results
    uint16_t adc_result = ADC;

    // Print the result on the LCD
    // Note: You'll need to implement or use an existing function to print to the LCD
    print_to_lcd(adc_result);
}