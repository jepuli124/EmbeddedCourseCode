#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
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
const int LCD_D7 = PD6;

int count(char* p);

uint16_t counter = 0;

ISR(ADC_vect)
{
    counter++;
    //counter = (counter >= 10) ? counter = 0 : ++counter;
    if (counter >= 100000) {
        lcd_gotoxy(0,0);
        PORTB ^= (1 << BUILTIN);
        char buf[20];
        itoa(counter, buf, 10);
        lcd_clrscr();
        lcd_puts(buf);
        counter = 0;
    }
    //print_to_lcd(ADC);
}


int main(void) {

    DDRB |= (1 << BUILTIN);
    // OUTPUTS CONTROL
    DDRB |= (1<<LCD_RS) | (1 << LCD_RW ) | (1 << LCD_EN ); 
    // OUTPUTS DATA
    DDRD |= (1 << LCD_D4 )| (1 << LCD_D5 )| (1 << LCD_D6 )| (1 << LCD_D7 );    
    // Thermistor input 
    DDRC &= ~(1 << THERMISTOR);


    TCCR0A = 0; // Clear the register
    TCCR0B = 0;
    TCNT0 = 0;

    // 
    TCCR0A |= (1 << WGM01) | (1 << COM0B0);
    
    OCR0A = 255;
    
    TIMSK0 |= (1 << OCIE0A);

    // Set compare match on the compare register A (OC0A)
    // Use toggle output on match compare
    
    // Try different prescalers in the TCCR0B
    // For example, set prescaler to 1024
    TCCR0B |= (1 << CS02) | (1 << CS00);


    // Set up the ADC
    ADMUX = 0; // Clear the register

    // Select voltage reference and input channel
   // For example, use AVCC as voltage reference and ADC3 as input channel
    ADMUX |= (1 << REFS0) | (1 << MUX1) | (1 << MUX0) ; // | (1 << ADLAR)

    // Enable the ADC, auto triggering, and the ADC interrupt
    ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // Set the auto trigger source (timer/counter0 compare match A)
    ADCSRB |= (1 << ADTS1) | (1 << ADTS0);
 
    sei();
    lcd_init(LCD_DISP_ON);

    while(1) {
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






// #define F_CPU 16000000UL

// #include <avr/io.h>
// #include <util/delay.h>
// #include <stdlib.h>
// #include <avr/interrupt.h>

// #include "lcd.h"

// const int THERMISTOR = PC3;

// const int LCD_RS = PB1;
// const int LCD_RW = PB2;
// const int LCD_EN = PB3;

// const int LCD_D4 = PD3;
// const int LCD_D5 = PD4;
// const int LCD_D6 = PD5;
// const int LCD_D7 = PD6;

// uint16_t res = 0;

// int count(char* p);

// ISR(ADC_vect) {
//     res = ADC;
//     char buf[16];
//     itoa(res, buf, 10);
//     lcd_clrscr();
//     lcd_puts(buf);
// }


// int main(void) {
    

//     // OUTPUTS CONTROL
//     DDRB |= (1<<LCD_RS) | (1 << LCD_RW ) | (1 << LCD_EN ); 
    
//     // OUTPUTS DATA
//     DDRD |= (1 << LCD_D4 )| (1 << LCD_D5 )| (1 << LCD_D6 )| (1 << LCD_D7 );
    
//     // Thermistor input 
//     DDRC &= ~(1 << THERMISTOR);
    

    
//     TCCR0A |= (1 << WGM01);
    
//     // OK
//     OCR0A = 255;
//     TCCR0A |= (1 << COM0A0);
//     TCCR0B |= (1 << CS01) | (1 << CS00);
    
//     TIMSK0 |= (1 << OCIE0A);

//     ADMUX |= (1 << REFS0) | (1 << MUX1) | (1 << MUX0);
    
//     ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADIE);
    
//     ADCSRB |= (1 << ADTS1) | (1 << ADTS0);
    
//     ADCSRA |= (1 << ADSC);

//     sei();
//     lcd_init(LCD_DISP_ON);
    
//     while(1) {
//         continue;
//     }

//     // char wait[] = "-_-_-_-_-";
//     // int n = count(wait);
    
//     // while(1) {
//     //     for (size_t i = 0; i < n; i++) {
//     //         lcd_putc(wait[i]);
//     //         _delay_ms(200);
//     //     }
//     // }
    
//     return 0;
// }


// int count(char* p) {
//     int count = 0;
//     while(p[count] != '\0') {
//         count++;
//     }
//     return count;
// }
