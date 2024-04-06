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

/*------------------------------*/
// BUZZER SETUP 
/*-----------------------------*/

const int PRESCALER_64 = 64;
const int PRESCALER_1 = 1;

/*------------------------------*/
// USART SETUP 
/*-----------------------------*/

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

/*------------------------------*/
// TIMER SETUP 
/*-----------------------------*/

volatile short is_timer_Ready = 0;
volatile uint16_t adc = 0; 

// When the ADC is complete, print the result in the LCD.
// You can save the conversion results in the ISR(interrupt_vector) sub-routine 
ISR(ADC_vect)
{
    // Debugging led
    PORTB ^= (1 << BUILTIN);
    is_timer_Ready = 1;
    adc = ADC;
}

/*------------------------------*/
// MAIN LOOP 
/*-----------------------------*/


int main(void) {

    DDRB |= (1 << LED_BUILTIN);
    PORTB |= (1 << LED_BUILTIN);

    /*------------------------------*/
    // USART ? SETUP 
    /*-----------------------------*/

    USART_Init(MYUBRR);

    stdout = &mystdout;
    stdin = &mystdin;


    /*------------------------------*/
    // BUZZER SETUP 
    /*-----------------------------*/

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

    TCNT1 = 0;
    

    /*------------------------------*/
    // TIMER SETUP 
    /*-----------------------------*/

    TCCR0A = 0;
    TCCR0B = 0;
  
    // WGM01 -> Set the operation mode to clear timer on compare CTC.
    // COM0A0 -> Toggle OC0A on Compare Match  |  Use toggle output on match compare. 
    //TCCR0A |= (1 << WGM01) | (1 << COM0A0);
    
    TCCR0A = 0x02;

    // Set the TOP to be 255
    OCR0A = 255;
    
    // compare match on the compare register A (OC0A).
    TIMSK0 |= (1 << OCIE0A);
    
    // Set prescaling to 1024
    TCCR0B |= (1 << CS02) | (1 << CS00);

  
    ADMUX = 0;
    
    // REFS0 -> AV_cc | MUX1, MUX0 -> Analog Channel 3 pin A3
    ADMUX |= (1 << REFS0) | (1 << MUX1) | (1 << MUX0) ; 

    //         ADC Enable |ADC StartConv| ADC AutoTrig | ADC InterrEn||ADC PreScaler 1 0 1 -> 32 
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE) | (1 << ADIE)  | (1 << ADPS2) | (1 << ADPS0);
    
    ADCSRB = 0;
    // Timer0 compare match A | 0 1 1
    ADCSRB |= (1 << ADTS1) | (1 << ADTS0);



    sei();

    /*while(12) {
        fprintf(stdin, "%s\n", "Hello world!");
        printf("Hello there!");
        PORTB &= ~(1 << LED_BUILTIN);
        _delay_ms(250);
        PORTB |= (1 << LED_BUILTIN);
        _delay_ms(250);
    } */

    /*------------------------------*/
    // MAIN LOOP 
    /*-----------------------------*/

    int state = 0;
    while (1) {

        switch (state)
        {
        case 0: // alarm is armed
            if (0) /*movement sensor output detected*/ {
                state = 2;
            }
            break;
        case 1: // movement detected
            if (0) /*password is correctly inputed*/ {
                state = 2;
            }
            if (0) /*timer ends before correct password is inputed*/ {
                state = 3;
            }
            break;
        case 2: // Alarm is disarmed
            if (0) /*BUTTON IS PRESSED*/ {
                state = 0;
            } 

            break;
        case 3: // Buzzer go brrrrr.
            TCCR1B = 0;
            TCCR1B = (1 << CS10) | (1 << WGM13);
            
            OCR1A = F_CPU / (2 * PRESCALER_1 * NOTE_C5);
            _delay_ms(750);
            state = 2;

            break;
        default:
            //we shouldn't be here?
            state = 2;
            break;
        }
        
    }

    return 0;
}

