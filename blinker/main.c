#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>


const int BUILTIN_UNO = PB5;

const int BUILTIN_MEGA = PB7;


int main(void) {

    DDRB |= (1 << BUILTIN_UNO);


    while(1) {
    
        PORTB |= (1 << BUILTIN_UNO);
        _delay_ms(500);

        PORTB &= ~(1 << BUILTIN_UNO);
        _delay_ms(650);
    
    }

    return 0;
}