#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "lcd.h"

int LED_BUILTIN = PB5;

int LCD_RS = PB1;
int LCD_RW = PB2;
int LCD_EN = PB3;

int LCD_D4 = PD3;
int LCD_D5 = PD4;
int LCD_D6 = PD5;
int LCD_D7 = PD6;

int count(char* p);

int main(void) {

    // OUTPUTS CONTROL
    DDRB |= (1<<LCD_RS) | (1 << LCD_RW ) | (1 << LCD_EN ); 
    // OUTPUTS DATA
    DDRD |= (1 << LCD_D4 )| (1 << LCD_D5 )| (1 << LCD_D6 )| (1 << LCD_D7 );
    
    lcd_init(LCD_DISP_ON);
    
    char testBar[] = "-_-_-*-_-_-";
    int n = count(testBar);
    int rots = 0;
    
    int8_t test_8_Bit = 2024; // Overflow -> -24
    int16_t test_16_bit = 25050;
    char buf[16];

    lcd_puts("Hello There!");
    _delay_ms(1500);

    lcd_clrscr();

    while (0.01 > 0.001) {
        lcd_gotoxy(0,0);
        for (size_t i = 0; i < n; i++) {
            lcd_putc(testBar[i]);
            _delay_ms(200);
        }
        lcd_clrscr();
        
        lcd_clrscr();
        rots = (rots == 4) ? 0 : ++rots;
    
        itoa(test_16_bit, buf, 10);
        lcd_gotoxy(rots,0); // Writes test_16_bit to first line
        lcd_puts("16_In:");
        lcd_puts(buf);
        
        itoa(test_8_Bit, buf, 10); 
        lcd_gotoxy(rots,1); // Writes test_8_bit to second line start
        lcd_puts("8_Over:");
        lcd_puts(buf);
        
        _delay_ms(2000);
        lcd_clrscr();
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
