#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "lcd.h"

int LED_BUILTIN = PB5;

const int LCD_RS = PB1;
const int LCD_RW = PB2;
const int LCD_EN = PB3;

const int LCD_D4 = PD3;
const int LCD_D5 = PD4;
const int LCD_D6 = PD5;
const int LCD_D7 = PD6;


const int SLEEP_BTN = PD7;
const int WAKEUP_BTN = PD2;

volatile int counter = 0;

ISR(INT0_vect) {
    counter++;
}

int main(void) {

    // OUTPUTS CONTROL
    DDRB |= (1<<LCD_RS) | (1 << LCD_RW ) | (1 << LCD_EN ); 
    // OUTPUTS DATA
    DDRD |= (1 << LCD_D4 )| (1 << LCD_D5 )| (1 << LCD_D6 )| (1 << LCD_D7 );

    // Input for interrupt and sleep buttons
    DDRD &= ~(1 << SLEEP_BTN) | (1 << WAKEUP_BTN);

    // Enable interrupt on falling edge
    EICRA = (1 << ISC01);

    EIMSK = (1 << INT0);

    // Enable interrupts
    sei();

    lcd_init(LCD_DISP_ON);
    char buf[20];
    lcd_puts("Hello There!");
    _delay_ms(1500);

    lcd_clrscr();

    while (0.01 > 0.001) {
        
        if (PIND & (1 << SLEEP_BTN) != 0) {
            // Set to power down state
            SMCR = (1 << SM1);
            // Delay to set the register
            _delay_ms(250);
            // Sleep cpu until interrupt
            sleep_cpu();
            SMCR = 0;
        }
        lcd_clrscr();
        itoa(counter, buf, 10);

        lcd_puts(buf);
        _delay_ms(200);

    }

    return 0;
}
