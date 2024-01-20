#define F_CPU 16000000UL

#include <util/delay.h>
#include <avr/io.h>


int main(void) {
	// DDRx -> Data Direction, inputs and outputs
	// Using pins 24,26 as inputs and 6,7 as outputs
	// 6  = PH3
	// 7  = PH4
	// 24 = PA2
	// 26 = PA4
	
	const int TRANS_A = PH3;
	const int TRANS_B = PH4;
	const int BTN_A = PA2;
	const int BTN_B = PA4;

	// Pin modes:
	DDRA &= ~( 1 << BTN_A) & ~(1 << BTN_B);  // Input
	DDRH |= (1 << TRANS_A) | (1 << TRANS_B); // Output
	
	// Read pins
	while(1) {
		// Read states of pins
		int BtnState_A = (PINA & (1 << BTN_A));
		int BtnState_B = (PINA & (1 << BTN_B));
		
		// PORTx write to port,  Pxn defines the port (x) and pin(n)
		if (BtnState_A) {
			PORTH |= (1 << TRANS_A);
			} else {
			PORTH &= ~(1 << TRANS_A);
		}

		if (BtnState_B) {
			PORTH |= (1 << TRANS_B);
			} else {
			PORTH &= ~(1 << TRANS_B);
		}
	}

	return 0;
}