#define F_CPU 16000000UL

#define FOSC 16000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1


#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <stdlib.h>
#include <stdio.h>

const int LED_BUILTIN = PB5;

static int uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = c;
    return 0;
}

static int uart_readchar(FILE *stream) {

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
}


int main(void) {

    USART_Init();

    init_uart();
    stdout = &mystdout;
    stdin = &mystdin;

    printf("Hello world!\n");
    
    return 0;
}

