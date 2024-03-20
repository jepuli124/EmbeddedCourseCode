#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "lcd.h"

int LED_BUILTIN = PB5;

// const int LCD_RS = PB1;
// const int LCD_RW = PB2;
// const int LCD_EN = PB3;
// 
// const int LCD_D4 = PD3;
// const int LCD_D5 = PD4;
// const int LCD_D6 = PD5;
// const int LCD_D7 = PD6;

const int LCD_RS = PH6;
const int LCD_RW = PB4;
const int LCD_EN = PB5;

const int LCD_D4 = PE5;
const int LCD_D5 = PG5;
const int LCD_D6 = PE3;
const int LCD_D7 = PH3;


const int SLEEP_BTN = PH4;
const int WAKEUP_BTN = PD0;


volatile int counter = 0;
volatile double timeCnt = 0;

ISR(INT0_vect) {
    counter++;
    // PORTB ^= (1 << LED_BUILTIN);
}

void goToSleep() {
    EICRA = 0;
    EIMSK = 0;
    EICRA = (1 << ISC01);
    // EICRA = 0x02;
    EIMSK = (1 << INT0);
    SMCR = 0b00000100;
    SMCR |= (1 << SE);
    _delay_ms(100);
    sleep_cpu();
    SMCR = 0;
}

int main(void) {

    // OUTPUTS CONTROL
    DDRB |= (1 << LCD_RW) | (1 << LCD_EN); 

    DDRE |= (1 << LCD_D4) | (1 << LCD_D6); 

    DDRG |= (1 << LCD_D5);
    
    DDRH |= (1 << LCD_RS) | (1 << LCD_D7); 
        
    // Input for interrupt and sleep buttons
    DDRD &= ~(1 << WAKEUP_BTN);
    DDRH &= ~(1 << SLEEP_BTN);

    sei();

    lcd_init(LCD_DISP_ON);
    char buf[20];

    lcd_clrscr();
    lcd_puts("Interrupt count:");
    lcd_gotoxy(0,1);

    _delay_ms(1000);

    while (0.01 > 0.001) {    
        timeCnt += 0.1;
        int s = (PINH & (1 << SLEEP_BTN));
        
        if (PINH & (1 << SLEEP_BTN)) {   
            goToSleep();
        }
        itoa(s, buf, 10);
        lcd_gotoxy(0,1);
        lcd_puts(buf);
            
        lcd_gotoxy(5,1);
        dtostrf(timeCnt, 10, 3, buf);
        lcd_puts(buf);
        _delay_ms(100);
    }

    return 0;
}
