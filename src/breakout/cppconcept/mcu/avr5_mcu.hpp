
#pragma once

#include "../intfs/mcu.hpp"
#include "../intfs/ports.hpp"
#include "../intfs/databus.hpp"
#include "avr5_regs.hpp"

extern "C" {
    __attribute((naked)) void __delay_us();
    void busy_wait_loop5(uint16_t count);
}

template<uintptr_t ddr_addr, uintptr_t port_addr, uint8_t field>
class avr5mcu_port {
public:
    void mode(port_mode m) {
        volatile uint8_t* ddr = reinterpret_cast<volatile uint8_t*>(ddr_addr);
        *ddr &= ~(1 << field);
        if (m == port_mode::output)
            *ddr |= (1 << field);
    }

    void set(bool value) {
        volatile uint8_t* port = reinterpret_cast<volatile uint8_t*>(port_addr);
        if (value)
            *port |= (1 << field);
        else
            *port &= ~(1 << field);
    }

    bool get() {
        volatile uint8_t* port = reinterpret_cast<volatile uint8_t*>(port_addr);
        return (*port & (1 << field)) != 0;
    }
};

template<digitalport dp_mosi, digitalport dp_miso, digitalport dp_sck, digitalport dp_ss>
class avr5_spi final {
    dp_mosi& mosi;
    dp_miso& miso;
    dp_sck& sck;
    dp_ss& ss;
public:
    avr5_spi(dp_mosi& mosi, dp_miso& miso, dp_sck& sck, dp_ss& ss) : 
        mosi(mosi), miso(miso), sck(sck), ss(ss) {
    }

    /* setup hardware SPI */
    void setup(uint32_t speed) {
        mosi.mode(port_mode::output);
        miso.mode(port_mode::input);
        sck.mode(port_mode::output);

        // SS must be high when setting master mode
        ss.mode(port_mode::output);
        ss.set(true);
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

    void write(uint8_t b) {
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

    uint8_t read() {
        //TODO: Verify correctness
        while (!SPSR->SPIF);
        return *SPDR;
    }

    bool has_data() {
        //TODO: Verify correctness
        return SPSR->SPIF;
    }

    bool start_transaction(uint16_t address) { return true; }
    void end_transaction() { return; }

    databus_protocol get_protocol() {
        return databus_protocol::SPI;
    }
};

template <mcu mcu>
class avr5_uart0 {
    mcu& mmcu;
public:
    avr5_uart0(mcu& mmcu) : mmcu(mmcu) {
        // send ^ to start measuring time in experiments
        setup(115200);
        enable();
        write('^');
    }

    void setup(uint32_t baud) {
        // use 2x
        UCSR0A->U2X0 = false;
        
        uint8_t multipl = 16u;
        if (UCSR0A->U2X0) {
            multipl = 8u;
        }

        auto div = uint32_t(baud) * multipl;
        auto velocity = (mmcu.clock() / div) - 1;

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
};

class avr5mcu_b0 : public avr5mcu_port<DDRB, PORTB, 0> {};
class avr5mcu_b1 : public avr5mcu_port<DDRB, PORTB, 1> {};
class avr5mcu_b2 : public avr5mcu_port<DDRB, PORTB, 2> {};
class avr5mcu_b3 : public avr5mcu_port<DDRB, PORTB, 3> {};
class avr5mcu_b4 : public avr5mcu_port<DDRB, PORTB, 4> {};
class avr5mcu_b5 : public avr5mcu_port<DDRB, PORTB, 5> {};
