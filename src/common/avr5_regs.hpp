
#pragma once
#include "../cppdeps.h"

const uint8_t PORTB = 0x25;
const uint8_t DDRB = 0x24;

/*
 * UART
 */

 struct UCSRA_t {
    uint8_t MPCM0:1;
    uint8_t U2X0:1;
    uint8_t UPE0:1;
    uint8_t DOR0:1;
    uint8_t FE0:1;
    uint8_t UDRE0:1;
    uint8_t TXC0:1;
    uint8_t RXC0:1;
};

struct UCSRB_t {
    uint8_t TXB80:1;
    uint8_t RXB80:1;
    uint8_t UCSZ02:1;
    uint8_t TXEN0:1;
    uint8_t RXEN0:1;
    uint8_t UDRIE0:1;
    uint8_t TXCIE0:1;
    uint8_t RXCIE0:1;
};

struct UCSRC_t {
    uint8_t UCPOL0:1;
    uint8_t UCSZ0:2;
    uint8_t USBS0:1;
    uint8_t UPM0:2;
    uint8_t UMSEL0:2;
};

// USART0 control status register A
extern volatile UCSRA_t *UCSR0A;

// USART0 control status register B
extern volatile UCSRB_t *UCSR0B;

// USART0 control status register C
extern volatile UCSRC_t *UCSR0C;

// USART0 baud rate register, 12 bits
extern volatile uint16_t *UBRR0;

// USART0 byte to send/received
extern volatile uint8_t *UDR0;

/*
 * SPI
 */

struct SPCR_t {
	uint8_t SPR0:1; // SPI clock rate 0
	uint8_t SPR1:1; // SPI clock rate 1
	uint8_t CPHA:1; // Clock phase
	uint8_t CPOL:1; // Clock polarity
	uint8_t MSTR:1; // Master
	uint8_t DORD:1; // Data order
	uint8_t SPE:1;  // SPI enable
	uint8_t SPIE:1; // SPI interruption enable
};

struct SPSR_t {
	uint8_t SPI2x:1; // Double SPI speed
	uint8_t _reserved:5;
	uint8_t WCOL:1;  // Write COLision flag
	uint8_t SPIF:1;  // SPI interrupt flag
};

// SPI Control Register
extern volatile SPCR_t *SPCR;

// SPI Status Register
extern volatile SPSR_t *SPSR;

// SPI Data Register
extern volatile uint8_t *SPDR;