
#pragma once

#include "../intfs/mcu.hpp"
#include "../intfs/ports.hpp"
#include "../intfs/databus.hpp"
#include "avr5_regs.hpp"

extern "C" {
    __attribute((naked)) void __delay_us();
    void busy_wait_loop5(uint16_t count);
}

class avr5mcu_port final {
    const uint8_t ddr_addr;
    const uint8_t port_addr;
    const uint8_t field;
public:
    avr5mcu_port(const uint8_t ddr_addr, const uint8_t port_addr, const uint8_t field):
        ddr_addr(ddr_addr), port_addr(port_addr), field(field) {
    }

    void mode(port_mode m) {
        volatile uint8_t* ddr = reinterpret_cast<volatile uint8_t*>(ddr_addr);
        *ddr &= ~(1 << field);
        if (m == port_mode::output)
            *ddr |= (1 << field);
    }

    __attribute__((always_inline)) void set(bool value) {
        volatile uint8_t* port = reinterpret_cast<volatile uint8_t*>(port_addr);
        if (value)
            *port |= (1 << field);
        else
            *port &= ~(1 << field);
    }

    __attribute__((always_inline)) bool get() {
        volatile uint8_t* port = reinterpret_cast<volatile uint8_t*>(port_addr);
        return (*port & (1 << field)) != 0;
    }
};

template<digitalport digitalport>
class avr5_spi final {
    digitalport *b3;
    digitalport *b4;
    digitalport *b5;
public:
    avr5_spi(digitalport *mosi, digitalport *miso, digitalport *sck) {
        b3 = mosi;
        b4 = miso;
        b5 = sck;
    }
    /* setup hardware SPI at:
       b3 = MOSI
       b4 = MISO
       b5 = SCK */
    void setup(uint32_t speed) {
        b3->mode(port_mode::output);
        b4->mode(port_mode::input);
        b5->mode(port_mode::output);
        SPCR->MSTR = true; // master
        
        //TODO: ignoring speed for now. Set to fsck/4 = 4Mhz
        SPSR->SPI2x = false;
        SPCR->SPR0 = false;
        SPCR->SPR1 = false;

        SPCR->CPOL = false;
        SPCR->CPHA = false;
    }

    void async_read_to(void *i) {}

    void enable() {
        SPCR->SPE = true;
    }

    void disable() {
        SPCR->SPE = false;
    }

    __attribute((always_inline)) void write(uint8_t b) {
        *SPDR = b;
        while (!SPSR->SPIF);
    }

    void write_array(char data[], uint8_t data_size) {
        uint8_t i = 0u;
        while (i < data_size) {
            *SPDR = (uint8_t)data[i];
            while (!SPSR->SPIF);
            i++;
        }
    }

    __attribute((always_inline)) uint8_t read() {
        //TODO: Verify correctness
        while (!SPSR->SPIF);
        return *SPDR;
    }

    __attribute((always_inline)) bool has_data() {
        //TODO: Verify correctness
        return SPSR->SPIF;
    }

    bool start_transaction(uint16_t address) { return true; }
    void end_transaction() { return; }

    __attribute((always_inline)) databus_protocol get_protocol() {
        return databus_protocol::SPI;
    }
};

class avr5_uart0 final {
    const uint32_t clock;
public:
    avr5_uart0(const uint32_t clock) : clock(clock) {
    }

    void setup(uint32_t baud) {
        // use 2x
        UCSR0A->U2X0 = false;
        
        uint8_t multipl = 16u;
        if (UCSR0A->U2X0) {
            multipl = 8u;
        }

        auto div = uint32_t(baud) * multipl;
        auto velocity = (clock / div) - 1;

        // set baudrate
        *UBRR0 = uint16_t(velocity);

        // 8 bit
        UCSR0B->UCSZ02 = false;
        UCSR0C->UCSZ0 = 0x3;

        // no parity
        UCSR0C->UPM0 = 0x0;

        // 1 stop bit
        UCSR0C->USBS0 = false;
    }

    void async_read_to(void *i) {
        //int_usart_rx = i;
        UCSR0B->RXCIE0 = true;
    }

    void enable() {
        // enable tx and rx
        UCSR0B->RXEN0 = true;
        UCSR0B->TXEN0 = true;
    }

    void disable() {
        // disable tx and rx
        UCSR0B->RXEN0 = false;
        UCSR0B->TXEN0 = false;
        // disable RX interruption
        UCSR0B->RXCIE0 = false;
    }

    void write(uint8_t b) {
        while (!UCSR0A->UDRE0);
        *UDR0 = b;
    }

    void write_array(char data[], uint8_t data_size) {
        uint8_t i = 0u;
        while (i < data_size) {
            write((uint8_t)data[i]);
            i++;
        }
    }

    uint8_t read() {
        while (!UCSR0A->RXC0);
        return *UDR0;    
    }

    bool has_data() {
        return UCSR0A->RXC0;
    }

    bool start_transaction(uint16_t address) { return true; }
    void end_transaction() { }
    
    databus_protocol get_protocol() {
        return databus_protocol::UART;
    }
};


class avr5mcu {
public:

    uint32_t clock() { 
        return 16000000;
    }

    void set_interruptions(bool enabled) {
        if (enabled) {
            asm ("sei");
        } else {
            asm ("cli");
        }
    }

	void wait_ms(uint16_t ms) {
        uint16_t count = (uint16_t)(clock() / 1280000) * ms - 1;
        busy_wait_loop5(count);
	}

    avr5mcu_port b0;
    avr5mcu_port b1;
    avr5mcu_port b2;
    avr5mcu_port b3;
    avr5mcu_port b4;
    avr5mcu_port b5;

    avr5_spi<avr5mcu_port> spi;
    avr5_uart0 uart0;

    avr5mcu(): 
        b0(DDRB, PORTB, 0),
        b1(DDRB, PORTB, 1),
        b2(DDRB, PORTB, 2),
        b3(DDRB, PORTB, 3),
        b4(DDRB, PORTB, 4),
        b5(DDRB, PORTB, 5),
        spi(&b3, &b4, &b5),
        uart0(this->clock()) {
    }
};
