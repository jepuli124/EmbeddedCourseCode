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

const int SS = PB2;     // SLAVE INPUT DPIN(10) SS
const int MOSI = PB3;   // SLAVE INPUT DPIN(11)
const int MISO = PB4;   // SLAVE OUTPUT DPIN(12) MISO
const int SCK = PB5;    // SLAVE INPUT DPIN(13)

// This good note:
// NOTE: USART = Universal Synchronous Asynchronous Receiver Transmitter
//       UART  = Universal Asynchronous Receiver Transmitter

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
    // Wait until the transmit buffer is empty
    while(!(UCSR0A & (1 << UDRE0))) {
        ;
    }
    
    // Get the received data from the buffer 
    return UDR0;
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
    
    // SET SLAVE OUTPUT
    DDRB |= (1 << MISO);

    // SET SLAVE INPUTS
    DDRB &= ~(1 << MOSI) & ~(1 << SS) & ~(1 << SCK);
    
    SPCR |= (1 << SPE) | (1 << SPR0);
    
    char send_str[STRINGMAX] = "Slave to Master";
    char recv_str[STRINGMAX] = "";
    
    USART_Init(MYUBRR);

    stdout = &mystdout;
    stdin = &mystdin;

    char* ptr = NULL;
    int i = 0;

    while(1) {
        // PORTB &= ~(1 << SS);
        for (ptr = send_str; *ptr != '\0'; ptr++) {
            SPDR = *ptr;
            while (!(SPSR & (1 << SPIF)));
            recv_str[i++] = SPDR;
        }
        recv_str[++i] = '\0';
        i = 0;
        // PORTB |= (1 << SS);
        printf("Slave > %s\n", recv_str);
        _delay_ms(250);
    }

    return 0;
}
