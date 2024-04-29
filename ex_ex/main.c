#define F_CPU 16000000UL

#define FOSC 16000000UL
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

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

volatile short is_timer_Ready = 1;
volatile int timeCounter = 0; // in milliseconds
volatile uint16_t adc = 0; 

// When the ADC is complete, print the result in the LCD.
// You can save the conversion results in the ISR(interrupt_vector) sub-routine 
ISR(ADC_vect)
{
    // Debugging led
    //PORTB ^= (1 << BUILTIN);
    adc = ADC;
    timeCounter++;
}

/*------------------------------*/
// MAIN LOOP 
/*-----------------------------*/


int main(void) {
    //noInterrupts(); // disable interrupt for the duration of setup
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

    // resets timer counter
    TCNT1 = 0;


    sei();
    //interrupts(); // enable interrupts back
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
                timeCounter = 0;
                is_timer_Ready = 0;
            }
            break;
        case 1: // movement detected
            if (0) /*password is correctly inputed*/ {
                state = 2;
            }
            if (timeCounter >= 10000) /*timer ends before correct password is inputed*/ {
                state = 3;
                is_timer_Ready = 1;
            }
            break;
        case 2: // Alarm is disarmed
            if (0) /*BUTTON IS PRESSED*/ {
                state = 0;
            } 

            break;
        case 3: // Buzzer go brrrrr.
            PORTB &= ~(1 << SS);
            TCCR1B = 0;
            TCCR1B = (1 << CS10) | (1 << WGM13);
            
            OCR1A = F_CPU / (2 * PRESCALER_1 * NOTE_C5);
            _delay_ms(750);
            
            PORTB |= ~(1 << SS);
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

