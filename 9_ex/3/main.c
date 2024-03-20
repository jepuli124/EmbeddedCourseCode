#define F_CPU 16000000UL
#define STRINGMAX 32


#define FOSC 16000000UL
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1


#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include <util/setbaud.h>
#include <stdio.h>

const int LED_BUILTIN = PB5;
const int READ_BTN = PB1;
const int WRITE_BTN = PB2;

unsigned int read_button(unsigned int reg, unsigned int bit);
void write_string(unsigned int ui_address, char* str);
char* read_string(char* dest, unsigned int ui_address);
void EEPROM_write(unsigned int ui_address, unsigned char uc_data);
unsigned char EEPROM_read(unsigned int ui_address);

static int uart_putchar(char c, FILE* stream);
static int uart_readchar(FILE* stream);

static int uart_putchar(char c, FILE* stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}

static int uart_readchar(FILE* stream) {
}

// Setup write and read
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
static FILE mystdin = FDEV_SETUP_STREAM(NULL, uart_readchar, _FDEV_SETUP_READ);


void USART_Init(unsigned int ubrr) {
    // Baud setup
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    // Transmitter / Receiver enable
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // Frame format 8b data 2b stop
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
    // PORTB &= ~(1 << LED_BUILTIN);
}


int main(void) {

    DDRB &= ~((1 << READ_BTN) | (1 << WRITE_BTN));

    char strings[2][STRINGMAX] = {"Seppo Taalasmaa", "Ismo Laitela"};
    char recv_str[STRINGMAX];
    unsigned int str_Index = 0;

    USART_Init(MYUBRR);

    stdout = &mystdout;
    stdin = &mystdin;

    while(1) {
        if (read_button(PINB, READ_BTN)) {
            write_string(0, strings[0]);
        }
        if (read_button(PINB, WRITE_BTN)) {
            write_string(0, strings[1]);
        }
        printf("%s\n", read_string(recv_str, 0));
        _delay_ms(1000);
    }

    return 0;
}

unsigned int read_button(unsigned int reg, unsigned int bit) {
    //for(int i = 0; i < 1000; i++) {}
    if (!(reg & (1 << bit))) { 
        return 0;
    }
        
    _delay_ms(10);
    return 1;
}

void write_string(unsigned int ui_address, char* str) {
    unsigned int i = 0;
    while (i < STRINGMAX){ // + ui_address -1) && str[i] != '\0'
        EEPROM_write(i+ui_address, str[i]);
        i++;
    }
    EEPROM_write(i, '\0');
}

char* read_string(char* dest, unsigned int ui_address) {
    unsigned int i = 0;
    while (i < STRINGMAX){ //  + ui_address - 1) && dest[i] != '\0'
        dest[i] = EEPROM_read(i+ui_address);
        i++;
    }
    dest[i] = '\0'; 
    return dest;
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
