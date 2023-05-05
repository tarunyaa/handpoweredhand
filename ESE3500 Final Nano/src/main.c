// NANO - TRANSMITTER

/* --------------------------------Initializations--------------------------------- */
#include <avr/io.h>
#include <stdio.h>
#include <avr/delay.h>
void ADCInit();
int readADC(void);
void InitializeUART(void);
void putString(char* ptr);
char String[25]; // Stores sequence of characters
uint8_t temp;
uint8_t q = 0;
uint8_t data_array[4];
uint8_t tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
uint8_t rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
void nrf24_init();
void nrf24_config(uint8_t channel, uint8_t pay_length);
void nrf24_rx_address(uint8_t * adr);
void nrf24_tx_address(uint8_t * adr);
void nrf24_send(uint8_t* value);
void nrf24_powerUpTx(void);
uint8_t nrf24_isSending();
uint8_t nrf24_lastMessageStatus();
uint8_t nrf24_retransmissionCount();
void    nrf24_powerUpRx();
#define NRF24_TRANSMISSON_OK 0
#define NRF24_MESSAGE_LOST   1
int ADCVal;
int value;
int count = 0;
/* -------------------------------Main-------------------------------------------- */
#define threshold 3 // position only updates if difference from input reading exceeds threshold
// stores current positions for each finger
int pos0 = 0;
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;

int main() {
    ADCInit();
    _delay_ms(1000);
    InitializeUART();
    nrf24_init();
    nrf24_config(2,5);
    nrf24_tx_address(tx_address);
    nrf24_rx_address(rx_address);
    sprintf(String, "\nGlove Ready\n");
    putString(String);

    while (1) {
        // input at ADC0
        ADMUX &= ~(1<<MUX3);
        ADMUX &= ~(1<<MUX2);
        ADMUX &= ~(1<<MUX1);
        ADMUX &= ~(1<<MUX0);
        ADCVal = readADC();
        if (ADCVal <= pos0 - threshold || ADCVal >= pos0 + threshold) {
            pos0 = ADCVal;
        }
        sprintf(String, "\nADC value for finger 1: %u\n", pos0);
        putString(String);
        data_array[0] = (unsigned int) pos0;

        // input at ADC1
        ADMUX &= ~(1<<MUX3);
        ADMUX &= ~(1<<MUX2);
        ADMUX &= ~(1<<MUX1);
        ADMUX |= (1<<MUX0);
        ADCVal = readADC();
        if (ADCVal <= pos1 - threshold || ADCVal >= pos1 + threshold) {
            pos1 = ADCVal;
        }
        sprintf(String, "\nADC value for finger 2: %u\n", pos1);
        putString(String);
        data_array[1] = (unsigned int) pos1;

        // input at ADC2
        ADMUX &= ~(1<<MUX3);
        ADMUX &= ~(1<<MUX2);
        ADMUX |= (1<<MUX1);
        ADMUX &= ~(1<<MUX0);
        ADCVal = readADC();
        if (ADCVal <= pos2 - threshold || ADCVal >= pos2 + threshold) {
            pos2 = ADCVal;
        }
        sprintf(String, "\nADC value for finger 3: %u\n", pos2);
        putString(String);
        data_array[2] = (unsigned int) pos2;

        // input at ADC3
        ADMUX &= ~(1<<MUX3);
        ADMUX &= ~(1<<MUX2);
        ADMUX |= (1<<MUX1);
        ADMUX |= (1<<MUX0);
        ADCVal = readADC();
        if (ADCVal <= pos3 - threshold || ADCVal >= pos3 + threshold) {
            pos3 = ADCVal;
        }
        sprintf(String, "\nADC value for finger 4: %u\n", pos3);
        putString(String);
        data_array[3] = (unsigned int) pos3;

        // input at ADC4
        ADMUX &= ~(1<<MUX3);
        ADMUX |= (1<<MUX2);
        ADMUX &= ~(1<<MUX1);
        ADMUX &= ~(1<<MUX0);
        ADCVal = readADC();
        if (ADCVal <= pos4 - threshold || ADCVal >= pos4 + threshold) {
            pos4 = ADCVal;
        }
        sprintf(String, "\nADC value for finger 5: %u\n", pos4);
        putString(String);
        data_array[4] = (unsigned int) pos4;

        nrf24_powerUpTx;

        /* Automatically goes to TX mode */
        nrf24_send(data_array);

        /* Wait for transmission to end */
        while(nrf24_isSending());

        /* Make analysis on last tranmission attempt */
        temp = nrf24_lastMessageStatus();

        if(temp == NRF24_TRANSMISSON_OK)
        {
            sprintf(String, "> Tranmission went OK\r\n");
            putString(String);
        }
        else if(temp == NRF24_MESSAGE_LOST)
        {
            sprintf(String, "> Message is lost ...\r\n");
            putString(String);
        }

        /* Retranmission count indicates the tranmission quality */
        temp = nrf24_retransmissionCount();
        sprintf(String, "> Retranmission count: %d\r\n",temp);
        putString(String);

        /* Optionally, go back to RX mode ... */
        nrf24_powerUpRx();

        value = (int) data_array[0];
        sprintf(String, "\ntransmitted 0: %u\n", value);
        putString(String);
        value = (int) data_array[1];
        sprintf(String, "\ntransmitted 1: %u\n", value);
        putString(String);
        value = (int) data_array[2];
        sprintf(String, "\ntransmitted 2: %u\n", value);
        putString(String);
        value = (int) data_array[3];
        sprintf(String, "\ntransmitted 3: %u\n", value);
        putString(String);
        value = (int) data_array[4];
        sprintf(String, "\ntransmitted 4: %u\n", value);
        putString(String);

        count ++;
        sprintf(String, "\n count: %u\n", count);
        putString(String);

        /* Wait a little ... */
        _delay_ms(10);

    }
}
