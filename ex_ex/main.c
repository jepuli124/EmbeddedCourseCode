#define F_CPU 16000000UL

#define FOSC 16000000UL
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define NOTE_C5 523
#define NOTE_A7 3520
#define NOTE_B2 123

#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>

const int LED_BUILTIN = PB5;
const int PRESCALER_64 = 64;
const int PRESCALER_1 = 1;

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

    DDRB |= (1 << LED_BUILTIN);
    PORTB |= (1 << LED_BUILTIN);

    USART_Init(MYUBRR);

    stdout = &mystdout;
    stdin = &mystdin;

    TCCR1B = 0;
    OCR1A = 0;

    // Buzzer and builtin led output
    DDRB |= (1 << BUZZER);
    DDRB |= (1 << BUILTIN);


    TCCR1A = (1 << COM1A0); // COM1A0
    TCCR1A = (1 << WGM10); // WGM10

    // Prescaler 1
    TCCR1B = (1 << CS10) | (1 << WGM13);

    // Enable COMPA interrupt
    TIMSK1 = (1 << OCIE1A);


    // Toggling builtin led works, lower note -> lower light output,
    //     higher note -> higher light output
    // Sets the 
    TCNT1 = 0;
    

    sei();

    while(12) {
        fprintf(stdin, "%s\n", "Hello world!");
        printf("Hello there!");
        PORTB &= ~(1 << LED_BUILTIN);
        _delay_ms(250);
        PORTB |= (1 << LED_BUILTIN);
        _delay_ms(250);
    }

    

    while (1) {
        // Reset to 0 and set prescaler 1
        TCCR1B = 0;
        TCCR1B = (1 << CS10) | (1 << WGM13);
        
        OCR1A = F_CPU / (2 * PRESCALER_1 * NOTE_C5);

        _delay_ms(500);

        OCR1A = F_CPU / (2 * PRESCALER_1 * NOTE_A7);

        _delay_ms(750);

        // Enable the PWM output by setting the clock source (prescaler 64) should
        // be same as constant PRESCALER64.
        TCCR1B = (1 << CS11) | (1 << CS10);
        OCR1A = F_CPU / (2 * PRESCALER_64 * NOTE_B2);
        
        _delay_ms(500);

    }

    return 0;
}

