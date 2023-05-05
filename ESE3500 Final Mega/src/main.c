// MEGA - RECEIVER

/* --------------------------------Initializations--------------------------------- */
#include <stdio.h>
#include <avr/delay.h>
void InitPWM();
void T0Init();
void InitLEDs();
void InitializeUART(void);
void putString(char* ptr);
void move(int[5]);
char String[25]; // Stores sequence of characters
uint8_t data_array[4];
uint8_t rx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
uint8_t tx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
void nrf24_init();
uint8_t nrf24_getStatus(void);
void nrf24_config(uint8_t channel, uint8_t pay_length);
void nrf24_rx_address(uint8_t * adr);
void nrf24_tx_address(uint8_t * adr);
void nrf24_powerUpRx(void);
uint8_t nrf24_dataReady();
void nrf24_getData(uint8_t* data);
#define NRF24_TRANSMISSON_OK 0
#define NRF24_MESSAGE_LOST   1
int ADC_values[5];

float getDuty(int i, int input_val, int motor_number);
/* -------------------------------Main-------------------------------------------- */

int main() {
    InitPWM();
    _delay_ms(1000);
    InitLEDs();
    InitializeUART();
    nrf24_init();
    nrf24_config(2,5);
    nrf24_tx_address(tx_address);
    nrf24_rx_address(rx_address);
    sprintf(String, "\nMechanical Hand Ready\n");
    putString(String);

    while (1) {

        if(nrf24_dataReady())
        {
            nrf24_getData(data_array);

            ADC_values[0] = (int) data_array[0];
            ADC_values[1] = (int) data_array[1];
            ADC_values[2] = (int) data_array[2];
            ADC_values[3] = (int) data_array[3];
            ADC_values[4] = (int) data_array[4];
            move(ADC_values);
        }
    }
}