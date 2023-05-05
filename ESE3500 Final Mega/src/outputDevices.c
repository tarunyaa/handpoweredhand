#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

void T0Init() {
    cli();
    DDRG |= (1<<DDG5); // output on pin 4 (G5)- OC0B PWM

    // prescale timer by 1024
    TCCR0B |= (1<<CS02);
    TCCR0B &= ~(1<<CS01);
    TCCR0B |= (1<<CS00);

    // phase correct PWM mode
    TCCR0B |= (1<<WGM02);
    TCCR0A &= ~(1<<WGM01);
    TCCR0A |= (1<<WGM00);

    // clear OC0B on compare match when up-counting, set when down-counting
    TCCR0A |= (1<<COM0B1);
    TCCR0A &= ~(1<<COM0B0);

    // OCR0A, OCR0B values
    OCR0A = 156; // 156 count up to for frequency of 50Hz (20 ms)
    OCR0B = 2.2 / 20.0 * OCR0A; // numerator range: 0.5-2.2
    sei();
}

void T1Init() {
    cli();
    DDRB |= (1<<DDB6); // output on pin 12 (B6)- OC1B PWM

    // prescale timer by 1024
    TCCR1B |= (1<<CS12);
    TCCR1B &= ~(1<<CS11);
    TCCR1B |= (1<<CS10);

    // phase correct PWM mode
    TCCR1B |= (1<<WGM13);
    TCCR1B &= ~(1<<WGM12);
    TCCR1A |= (1<<WGM11);
    TCCR1A |= (1<<WGM10);

    // clear OC1B on compare match when up-counting, set when down-counting
    TCCR1A |= (1<<COM1B1);
    TCCR1A &= ~(1<<COM1B0);

    // OCR1A, OCR1B values
    OCR1A = 156; // 156 count up to for frequency of 50Hz (20 ms)
    OCR1B = 2.2 / 20.0 * OCR1A; // numerator range: 0.5-2.2
    sei();
}

void T2Init() {
    cli();
    DDRH |= (1<<DDH6); // output on pin 9 (H6)- OC2B PWM

    // prescale timer by 1024
    TCCR2B |= (1<<CS22);
    TCCR2B |= (1<<CS21);
    TCCR2B |= (1<<CS20);

    // phase correct PWM mode
    TCCR2B |= (1<<WGM22);
    TCCR2A &= ~(1<<WGM21);
    TCCR2A |= (1<<WGM20);

    // clear OC2B on compare match when up-counting, set when down-counting
    TCCR2A |= (1<<COM2B1);
    TCCR2A &= ~(1<<COM2B0);

    // OCR2A, OCR2B values
    OCR2A = 156; // 156 count up to for frequency of 50Hz (20 ms)
    OCR2B = 2.2 / 20.0 * OCR2A; // numerator range: 0.5-2.2
    sei();
}

void T3Init() {
    cli();
    DDRE |= (1<<DDE4); // output on pin 2 (E4)- OC3B PWM

    // prescale timer by 1024
    TCCR3B |= (1<<CS32);
    TCCR3B &= ~(1<<CS31);
    TCCR3B |= (1<<CS30);

    // phase correct PWM mode
    TCCR3B |= (1<<WGM33);
    TCCR3B &= ~(1<<WGM32);
    TCCR3A |= (1<<WGM31);
    TCCR3A |= (1<<WGM30);

    // clear OC3B on compare match when up-counting, set when down-counting
    TCCR3A |= (1<<COM3B1);
    TCCR3A &= ~(1<<COM3B0);

    // OCR3A, OCR3B values
    OCR3A = 156; // 156 count up to for frequency of 50Hz (20 ms)
    OCR3B = 2.2 / 20.0 * OCR3A; // numerator range: 0.5-2.2
    sei();
}

void T4Init() {
    cli();
    DDRH |= (1<<DDH4); // output on pin 7 (H4)- OC4B PWM

    // prescale timer by 1024
    TCCR4B |= (1<<CS42);
    TCCR4B &= ~(1<<CS41);
    TCCR4B |= (1<<CS40);

    // phase correct PWM mode
    TCCR4B |= (1<<WGM43);
    TCCR4B &= ~(1<<WGM42);
    TCCR4A |= (1<<WGM41);
    TCCR4A |= (1<<WGM40);

    // clear OC4B on compare match when up-counting, set when down-counting
    TCCR4A |= (1<<COM4B1);
    TCCR4A &= ~(1<<COM4B0);

    // OCR4A, OCR4B values
    OCR4A = 156; // 156 count up to for frequency of 50Hz (20 ms)
    OCR4B = 2.2 / 20.0 * OCR4A; // numerator range: 0.5-2.2
    sei();
}

void InitPWM() {
    T0Init();
    T1Init();
    T2Init();
    T3Init();
    T4Init();
}

void InitLEDs() {
    DDRA |= (1<<DDA1); // pin 23
    DDRA |= (1<<DDA3); // pin 25
    DDRA |= (1<<DDA5); // pin 27
    DDRA |= (1<<DDA7); // pin 29
    DDRC |= (1<<DDC6); // pin 31
}

int val;
char String[25]; // Stores sequence of characters
#define STRAIGHT0 60
#define BENT0 42
#define STRAIGHT1 116
#define BENT1 65
#define STRAIGHT2 57
#define BENT2 29
#define STRAIGHT3 133
#define BENT3 84
#define STRAIGHT4 119
#define BENT4 82

void putString(char* ptr);

float getDuty(int input_val, int motor_number) {
    int straight = 0; // these values should not finish as zero
    int bent = 0;
    if (motor_number == 0) {
        straight = STRAIGHT0;
        bent = BENT0;
    } else if (motor_number == 1) {
        straight = STRAIGHT1;
        bent = BENT1;
    } else if (motor_number == 2) {
        straight = STRAIGHT2;
        bent = BENT2;
    } else if (motor_number == 3) {
        straight = STRAIGHT3;
        bent = BENT3;
    } else if (motor_number == 4) {
        straight = STRAIGHT4;
        bent = BENT4;
    }
    int diff = input_val - bent;
    if (diff < 0) {
        diff = 0;
    }
    return 1.0 * diff / (straight - bent) * (2.2 - 0.5) + 0.5;
}

//int flex;
int old; // used to check whether OCRnB has changed
void move(int ADC_values[5]) {

    old = OCR0B;
    //flex = flexSensorInput();
    OCR0B = getDuty(ADC_values[0], 0) / 20.0 * OCR0A; // adjust servo position
    if (old != OCR0B) {
        PORTA |= (1<<PORTA1);
    } else {
        PORTA &= ~(1<<PORTA1);
    }

    old = OCR1B;
    OCR1B = getDuty(ADC_values[1], 1) / 20.0 * OCR1A; // adjust servo position
    if (old != OCR1B) {
        PORTA |= (1<<PORTA3);
    } else {
        PORTA &= ~(1<<PORTA3);
    }

    old = OCR2B;
    OCR2B = getDuty(ADC_values[2], 2) / 20.0 * OCR2A; // adjust servo position
    if (old != OCR2B) {
        PORTA |= (1<<PORTA5);
    } else {
        PORTA &= ~(1<<PORTA5);
    }

    old = OCR3B;
    OCR3B = getDuty(ADC_values[3], 3) / 20.0 * OCR3A; // adjust servo position
    if (old != OCR3B) {
        PORTA |= (1<<PORTA7);
    } else {
        PORTA &= ~(1<<PORTA7);
    }

    old = OCR4B;
    OCR4B = getDuty(ADC_values[4], 4) / 20.0 * OCR4A; // adjust servo position
    if (old != OCR4B) {
        PORTC |= (1<<PORTC6);
    } else {
        PORTC &= ~(1<<PORTC6);
    }
}