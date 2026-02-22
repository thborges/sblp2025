
#include "avr5_regs.hpp"

// SPI

// SPI Control Register
volatile SPCR_t *SPCR = (volatile SPCR_t*)0x4c;

// SPI Status Register
volatile SPSR_t *SPSR = (volatile SPSR_t*)0x4d;

// SPI Data Register
volatile uint8_t *SPDR = (volatile uint8_t*)0x4e;

// USART0

// USART0 control status register A
volatile UCSRA_t *UCSR0A = (volatile UCSRA_t*)0xc0;

// USART0 control status register B
volatile UCSRB_t *UCSR0B = (volatile UCSRB_t*)0xc1;

// USART0 control status register C
volatile UCSRC_t *UCSR0C = (volatile UCSRC_t*)0xc2;

// USART0 baud rate register, 12 bits
volatile uint16_t *UBRR0 = (volatile  uint16_t*)0xc4;

// USART0 byte to send/received
volatile uint8_t *UDR0 = (volatile uint8_t*)0xc6;
