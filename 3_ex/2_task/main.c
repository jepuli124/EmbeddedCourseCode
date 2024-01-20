#define F_CPU 16000000UL

#include <util/delay.h>
#include <avr/io.h>


int main(void) {
    // port B -> pins: 8,9,10,11,12,13
    // port D -> pins: 0, 1, 2, 3, 4, 5, 6, 7
    // DDRx -> Data Direction, inputs and outputs    
    // Using pins 3,5 as inputs and 9,10 as outputs
    const int TRANS_B = PB1;
    const int TRANS_C = PB2;
    const int B_BTN = PD3;
    const int C_BTN = PD5;

    // Pinmodes:
    DDRD &= ~( 1 << B_BTN) & ~(1 << C_BTN);  // Input
    DDRB |= (1 << TRANS_B) | (1 << TRANS_C); // Output
    
    // Read pins
    while(1) {
        // Read states of pins
        int B_BtnState = (PIND & (1 << B_BTN));
        int C_BtnState = (PIND & (1 << C_BTN));
        
        // PORTx write to port,  Pxn defines the port (x) and pin(n)
        if (B_BtnState) {
            PORTB |= (1 << TRANS_B);
        } else {
            PORTB &= ~(1 << TRANS_B);
        }

        if (C_BtnState) {
            PORTB |= (1 << TRANS_C);
        } else {
            PORTB &= ~(1 << TRANS_C);
        }
    }

    return 0;
}

