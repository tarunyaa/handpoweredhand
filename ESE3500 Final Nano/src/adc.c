#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

void putString(char* ptr);
char String[10];

void ADCInit() {
   cli();
   PRR &= ~(1<<PRADC); // turn on ADC

   // prescale ADC timer by 128
   ADCSRA |= (1<<ADPS0);
   ADCSRA |= (1<<ADPS1);
   ADCSRA |= (1<<ADPS2);

   // AVcc as reference
   ADMUX |= (1<<REFS0);
   ADMUX &= ~(1<<REFS1);

   // free running mode
   ADCSRB &= ~(1<<ADTS0);
   ADCSRB &= ~(1<<ADTS1);
   ADCSRB &= ~(1<<ADTS2);

   DIDR0 = 0x00; // digital input disable for ADC0-ADC7

   ADCSRA |= (1<<ADEN); // ADC enable
   sei();
}

int readADC(void) {
   ADCSRA |= (1<<ADSC); // start conversion
   while (ADCSRA & (1 << ADSC)); // wait until it is finished
   return ADC / 4; // decrease resolution by factor of 4
}
