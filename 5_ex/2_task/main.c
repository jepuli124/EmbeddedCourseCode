// Clock speed of Arduino Uno and MEGA (16MHz) this is needed to synchronize the correct _delay_ms()
#define F_CPU 16000000UL
#define TOP_FLOOR 5
#define TIME_DOORS_STAY_OPEN 5000
#define TIME_PER_FLOOR 500

int state = 1;
int currFloor = 1;
int trgtFloor = 1;
// Headers
#include <avr/io.h>
#include <util/delay.h>


int main(void) {
    int pin2 = PD2;
    int pin3 = PD3;
    int pin4 = PD4;
    int pin5 = PD5;
    int pin6 = PD6;
    int pin7 = PD7;
    int pin8 = PB0;
    int pin9 = PB1;
    int pin10 = PB2; 
    int pin11 = PB3;
    int pin12 = PB4;
    int pin13 = PB5; 
    // digital pins 8 - 11 are port B
// digital pins:   8             9             10            11
    DDRB |= (1 << pin8) |(1 << pin9) |(1 << pin10) |(1 << pin11) |(1 << pin12) |(1 << pin13);
    DDRD &= ~( 1 << pin2) & ~( 1 << pin3) & ~( 1 << pin4) & ~( 1 << pin5) & ~( 1 << pin6);
    DDRD |= (1 << pin7);
      
    while(1) {
        /*for (short i = 0; i < 4; i++) {
            PORTB |= (1 << i); // Turns the led on (sets the ith bit to 1)
            _delay_ms(1250);
            PORTB &= ~(1 << i); // Turns the led off (sets the ith bit to 0)
            _delay_ms(200);
        }*/
        /*PORTB &= ~( 1 << PB7);
        _delay_ms(200);
        PORTB |= ( 1 << PB7);
        _delay_ms(200); */
        int floor1 = (PIND & (1 << pin2));
        int floor2 = (PIND & (1 << pin3));
        int floor3 = (PIND & (1 << pin4));
        int floor4 = (PIND & (1 << pin5));
        int floor5 = (PIND & (1 << pin6));
        switch (currFloor)
        {
        case 1:
            PORTB |= ( 1 << pin9);
            PORTB &= ~( 1 << pin10);
            PORTB &= ~( 1 << pin11);
            PORTB &= ~( 1 << pin12);
            PORTB &= ~( 1 << pin13);
            break;
        case 2:
            PORTB |= ( 1 << pin10);
            PORTB &= ~( 1 << pin9);
            PORTB &= ~( 1 << pin11);
            PORTB &= ~( 1 << pin12);
            PORTB &= ~( 1 << pin13);
            break;
        case 3:
            PORTB |= ( 1 << pin11);
            PORTB &= ~( 1 << pin10);
            PORTB &= ~( 1 << pin9);
            PORTB &= ~( 1 << pin12);
            PORTB &= ~( 1 << pin13);
            break;
        case 4:
            PORTB |= ( 1 << pin12);
            PORTB &= ~( 1 << pin10);
            PORTB &= ~( 1 << pin11);
            PORTB &= ~( 1 << pin9);
            PORTB &= ~( 1 << pin13);
            break;
        case 5:
            PORTB |= ( 1 << pin13);
            PORTB &= ~( 1 << pin10);
            PORTB &= ~( 1 << pin11);
            PORTB &= ~( 1 << pin12);
            PORTB &= ~( 1 << pin9);
            break;
        
        default:
            break;
        }
        if (state == 2 || state == 3){
            PORTD |= ( 1 << pin7);
            
        } else {
            PORTD &= ~( 1 << pin7);
            
        }

        if (state == 4) PORTB |= ( 1 << pin8);
        else PORTB &= ~( 1 << pin8);

        switch(state) {
            case 0:
                state = 1;
                currFloor = 1;
                trgtFloor = 1;
            case 1:
            if (floor1 || floor2 || floor3 || floor4 || floor5) {
                if (floor1){ trgtFloor = 1;}
                if (floor2){ trgtFloor = 2;}
                if (floor3){ trgtFloor = 3;}
                if (floor4){ trgtFloor = 4;}
                if (floor5){ trgtFloor = 5;}
                
                if (trgtFloor < 1 || trgtFloor > TOP_FLOOR) {
                    state = 0;
                    break;
                }
                else if (trgtFloor > currFloor) {
                state = 2;
                break;
                }
                else if (trgtFloor < currFloor) {
                state = 3;
                break;
                }
                else if (trgtFloor == currFloor) {
                state = 4;
                break;
                }
            }
            break;
            case 2:
            if (currFloor < trgtFloor) {
                _delay_ms(TIME_PER_FLOOR); // time taken to travel 1 currFloor
                currFloor++;
                }
                else {
                state = 4;
                }
                break;
            case 3:
                if (currFloor > trgtFloor) {
                _delay_ms(TIME_PER_FLOOR); // time taken to travel 1 currFloor
                currFloor--;
                }
                else {state = 4;}
                break;
            case 4:
                _delay_ms(TIME_DOORS_STAY_OPEN); // doors stay open for 5 seconds
                state = 1;
                break;
        }

    }

    return 0;
}


/*
 COMMANDS TO COMPILE THE PROGRAM:
 Compiler Optimize, ard cpu freq,   cpu,         programmer, output test.o and source .c
$ avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o test.o test.c
 Compiler,  cpu,          this   to that
$ avr-gcc -mmcu=atmega328p test.o -o test
 object to test.hex file
$ avr-objcopy -O ihex -R.eeprom test test.hex

 bit banger:      programmer,  CPU,         port to bang,   baudrate, write to flash test.hex
$ avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:test.hex

Alternatively:

  avr-gcc -DF_CPU=16000000UL -mmcu=atmega328p -std=c99 <source filename>.c -o <object filename>.elf
  avr-objcopy -O ihex -j .text -j .data <object filename>.elf <output filename>.hex
  avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:<output filename>.hex

*/
