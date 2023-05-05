#ifndef UESE_350_FINAL_PROJECT_FOR_NANO_NRF24_H
#define UESE_350_FINAL_PROJECT_FOR_NANO_NRF24_H
/*
* ----------------------------------------------------------------------------
* “THE COFFEEWARE LICENSE” (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
* This library is based on this library:
*   https://github.com/aaronds/arduino-nrf24l01
* Which is based on this library:
*   http://www.tinkerer.eu/AVRLib/nRF24L01
* -----------------------------------------------------------------------------
*/
#include <avr/io.h>
#include "nRF24L01.h"
#include <stdint.h>

#define LOW 0
#define HIGH 1
#define set_bit(reg,bit) reg |= (1<<bit)
#define clr_bit(reg,bit) reg &= ~(1<<bit)
#define check_bit(reg,bit) (reg&(1<<bit))

#define nrf24_ADDR_LEN 5
#define nrf24_CONFIG ((1<<EN_CRC)|(0<<CRCO))

#define NRF24_TRANSMISSON_OK 0
#define NRF24_MESSAGE_LOST   1

/* adjustment functions */
void    nrf24_init();
void    nrf24_rx_address(uint8_t* adr);
void    nrf24_tx_address(uint8_t* adr);
void    nrf24_config(uint8_t channel, uint8_t pay_length);

/* state check functions */
uint8_t nrf24_dataReady();
uint8_t nrf24_isSending();
uint8_t nrf24_getStatus();
uint8_t nrf24_rxFifoEmpty();

/* core TX / RX functions */
void    nrf24_send(uint8_t* value);
void    nrf24_getData(uint8_t* data);

/* use in dynamic length mode */
uint8_t nrf24_payloadLength();

/* post transmission analysis */
uint8_t nrf24_lastMessageStatus();
uint8_t nrf24_retransmissionCount();

/* Returns the payload length */
uint8_t nrf24_payload_length();

/* power management */
void    nrf24_powerUpRx();
void    nrf24_powerUpTx();
void    nrf24_powerDown();

/* low level interface ... */
uint8_t spi_transfer(uint8_t tx);
void    nrf24_transmitSync(uint8_t* dataout,uint8_t len);
void    nrf24_transferSync(uint8_t* dataout,uint8_t* datain,uint8_t len);
void    nrf24_configRegister(uint8_t reg, uint8_t value);
void    nrf24_readRegister(uint8_t reg, uint8_t* value, uint8_t len);
void    nrf24_writeRegister(uint8_t reg, uint8_t* value, uint8_t len);

/* -------------------------------------------------------------------------- */
/* You should implement the platform specific functions in your code */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* In this function you should do the following things:
*    - Set MISO pin input
*    - Set MOSI pin output
*    - Set SCK pin output
*    - Set CS pin output
*    - Set CE pin output     */
/* -------------------------------------------------------------------------- */
extern void nrf24_setupPins() {
   // Set CE pin output (UNO - 7, MEGA - 7)
   DDRD |= (1<<DDD7);
#define NRF24_CE_PORT PORTD
#define NRF24_CE_PIN PORTD7

   // Set CS pin output (UNO - 8, MEGA - 8)
   DDRB |= (1<<DDB0);
#define NRF24_CS_PORT PORTB
#define NRF24_CS_PIN PORTB0

   // Set MISO pin output (UNO - 12, MEGA - 50)
   DDRB &= ~(1<<DDB4);
#define NRF24_MISO_PORT PINB
#define NRF24_MISO_PIN PINB4

   // Set MOSI pin output (UNO - 11, MEGA - 51)
   DDRB |= (1<<DDB3);
#define NRF24_MOSI_PORT PORTB
#define NRF24_MOSI_PIN PORTB3

   // Set SCK pin output (UNO - 13, MEGA - 52)
   DDRB |= (1<<DDB5);
#define NRF24_SCK_PORT PORTB
#define NRF24_SCK_PIN PORTB5
}


/* -------------------------------------------------------------------------- */
/* nrf24 CE pin control function
*    - state:1 => Pin HIGH
*    - state:0 => Pin LOW     */
/* -------------------------------------------------------------------------- */
extern void nrf24_ce_digitalWrite(uint8_t state) {
   if (state) {
       NRF24_CE_PORT |= (1 << NRF24_CE_PIN);
   }
   else {
       NRF24_CE_PORT &= ~ (1 << NRF24_CE_PIN);
   }
}

/* -------------------------------------------------------------------------- */
/* nrf24 CE pin control function
*    - state:1 => Pin HIGH
*    - state:0 => Pin LOW     */
/* -------------------------------------------------------------------------- */
extern void nrf24_csn_digitalWrite(uint8_t state) {
   if (state) {
       NRF24_CS_PORT |= (1 << NRF24_CS_PIN);
   }
   else {
       NRF24_CS_PORT &= ~ (1 << NRF24_CS_PIN);
   }
}

/* -------------------------------------------------------------------------- */
/* nrf24 SCK pin control function
*    - state:1 => Pin HIGH
*    - state:0 => Pin LOW     */
/* -------------------------------------------------------------------------- */
extern void nrf24_sck_digitalWrite(uint8_t state) {
   if (state) {
       NRF24_SCK_PORT |= (1 << NRF24_SCK_PIN);
   }
   else {
       NRF24_SCK_PORT &= ~ (1 << NRF24_SCK_PIN);
   }
}

/* -------------------------------------------------------------------------- */
/* nrf24 MOSI pin control function
*    - state:1 => Pin HIGH
*    - state:0 => Pin LOW     */
/* -------------------------------------------------------------------------- */
extern void nrf24_mosi_digitalWrite(uint8_t state) {
   if (state) {
       NRF24_MOSI_PORT |= (1 << NRF24_MOSI_PIN);
   }
   else {
       NRF24_MOSI_PORT &= ~ (1 << NRF24_MOSI_PIN);
   }
}

/* -------------------------------------------------------------------------- */
/* nrf24 MISO pin read function
/* - returns: Non-zero if the pin is high */
/* -------------------------------------------------------------------------- */
extern uint8_t nrf24_miso_digitalRead() {
   if (NRF24_MISO_PORT & (1<<NRF24_MISO_PIN)) {
       return 1;
   } else {
       return 0;
   }
}

#endif //UESE_350_FINAL_PROJECT_FOR_NANO_NRF24_H