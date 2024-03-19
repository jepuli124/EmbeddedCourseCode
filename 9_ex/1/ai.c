#include <avr/io.h>
#include <util/setbaud.h>
#include <stdio.h>

void USART_init(void) {
    UBRR0H = UBRRH_VALUE; // set baud rate
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= (1 << U2X0); // enable double speed operation
#else
    UCSR0A &= ~(1 << U2X0); // disable double speed operation
#endif

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Use 8-bit character sizes
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);   // Turn on the transmission and reception circuitry
}

void USART_tx_string(char *data) {
    while ((*data != '\0')) {
        while (!(UCSR0A & (1 << UDRE0))); // Wait for empty transmit buffer
        UDR0 = *data;
        data++;
    }
}

int main(void) {
    USART_init();  // initialize USART
    char string[] = "Hello World";  // string to send

    while(1) {
        USART_tx_string(string);  // transmit string
    }

    return 0;
}