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
void isr(ADC_vect) {


    return;
}


int main(void) {

    // OUTPUTS CONTROL
    DDRB |= (1<<LCD_RS) | (1 << LCD_RW ) | (1 << LCD_EN ); 
    
    // OUTPUTS DATA
    DDRD |= (1 << LCD_D4 )| (1 << LCD_D5 )| (1 << LCD_D6 )| (1 << LCD_D7 );
    
    // Thermistor input 
    DDRC &= ~(1 << THERMISTOR);

    lcd_init(LCD_DISP_ON);
    lcd_puts("Why yes, yes indeed!");

    while (0.01 > 0.001) {
        continue;
    }

    return 0;
}


int count(char* p) {
    int count = 0;
    while(p[count] != '\0') {
        count++;
    }
    return count;
}
