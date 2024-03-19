#define F_CPU 16000000UL
#define STRINGMAX 32

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>


const int LED_BUILTIN = PB5;
const int READ_BTN = PB1;
const int WRITE_BTN = PB2;


bool read_button(uint reg, uint bit) {
    for(int i = 0; i < 1000; i++) {
        if (reg & (1 << bit)){ return false };
    }
    return true;
}

void EEPROM_write(unsigned int ui_address, unsigned char uc_data) {
    while(EECR & (1 << EEPE));
    EEAR = ui_address;
    EEDR = uc_data;
    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);
}


unsigned char EEPROM_read(unsigned int ui_address) {
    
    while(EECR & (1 << EEPE));
    EEAR = ui_address;
    EECR |= (1 << EERE);
    return EEDR;
}

int main(void) {
    DDRB &= ~(1 << READ_BTN) & (1 << WRITE_BTN);

    while(1) {
        if (read_button(PINB, READ_BTN)) {
            char c;
            int i = 0;
            do {
                c = EEPROM_read(i);
                printf("%c",c);
                i++;
            } while (c != '\0'); 
        }
        if (read_button(PINB, WRITE_BTN)) {
            char c[STRINGMAX] = "Seppo taalasmaa";
            for (int i = 0; i < STRINGMAX; i++) {
                EEPROM_write(i, c[i]);
            }
        }
    }

    return 0;
}
