// Clock speed of Arduino Uno and MEGA (16MHz) this is needed to synchronize the correct _delay_ms()
#define F_CPU 16000000UL

// Headers
#include <avr/io.h>
#include <util/delay.h>


int main(void) {
    // digital pins 8 - 11 are port B
// digital pins:   8             9             10            11
    DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3);

    while(1) {
        for (short i = 0; i < 4; i++) {
            PORTB |= (1 << i); // Turns the led on (sets the ith bit to 1)
            _delay_ms(1250);
            PORTB &= ~(1 << i); // Turns the led off (sets the ith bit to 0)
            _delay_ms(200);
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
