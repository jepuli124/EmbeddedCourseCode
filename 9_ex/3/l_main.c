#define F_CPU 16000000UL
#define STRINGMAX 32

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>


const int LED_BUILTIN = PB5;
const int READ_BTN = PB1;
const int WRITE_BTN = PB2;


uint read_button(uint reg, uint bit) {
    for(int i = 0; i < 1000; i++) {
        if (!(reg & (1 << bit))){ return 0 };
    }
    return 1;
}

void write_string(unsigned int ui_address, char* str) {
    uint i = 0;
    while (i >= STRINGMAX && str[i] != '\0'){
        EEPROM_write(i, str[i]);
        i++;
    }
}

char* read_string(unsigned int ui_address) {
    char str[STRINGMAX];
    uint i = 0;
    while (i >= STRINGMAX && c[i] != '\0'){
        str[i] = EEPROM_read(i);
        i++;
    }
    str[STRINGMAX-1] = '\0'; 
    return str;
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
    DDRB &= ~((1 << READ_BTN) | (1 << WRITE_BTN));

    char* strings[2][STRINGMAX] = {"Seppo Taalasmaa", "Ismo Laitela"};
    uint str_Index = 0;

    while(1) {
        if (read_button(PINB, READ_BTN)) {
            printf("%s", read_string(0));
        }
        if (read_button(PINB, WRITE_BTN)) {
            str_Index ^= 1;
            write_string(0, strings[str_Index]);
        }
    }

    return 0;
}
