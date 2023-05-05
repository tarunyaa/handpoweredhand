#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

void InitializeUART(void) {
    cli();
    UBRR0H = (unsigned char)(BAUD_PRESCALER>>8);
    UBRR0L = (unsigned char)BAUD_PRESCALER;

    //UCSR0B |= (1<<RXEN3); // enable receiver
    //UCSR0B |= (1<<TXEN3); // enable transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // 8 data bits format
    //UCSR3C |= (1<<UCSZ31);
    //UCSR3C |= (1<<UCSZ30);

    // 2 stop bits
    //UCSR3C |= (1<<USBS3);

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //

    sei();
}

uint8_t receive(void) {
    // waits for buffer flag to indicate empty
    while (!(UCSR0A & (1<<RXC0)));
    return UDR0;
}

void send(unsigned char byte) {
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = byte;
}

void putString(char* ptr) {
    while (*ptr != 0x00) { // while we are not at the end of the string
        send(*ptr); // value at that pointer
        ptr++; // advance pointer
    }
}