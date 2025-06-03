
#include "../intfs/mcu.hpp"
#include "../intfs/ports.hpp"
#include "../intfs/databus.hpp"
#include "avr5_regs.hpp"

extern "C" __attribute((naked)) void __delay_us();
extern "C" void busy_wait_loop5(uint16_t count);

template<uintptr_t ddr_addr, uintptr_t port_addr, uint8_t field>
class avr5mcu_port : public digitalport {
public:
    void mode(port_mode m) override {
        volatile uint8_t* ddr = reinterpret_cast<volatile uint8_t*>(ddr_addr);
        *ddr &= ~(1 << field);
        if (m == port_mode::output)
            *ddr |= (1 << field);
    }

    void set(bool value) override {
        volatile uint8_t* port = reinterpret_cast<volatile uint8_t*>(port_addr);
        if (value)
            *port |= (1 << field);
        else
            *port &= ~(1 << field);
    }

    bool get() override {
        volatile uint8_t* port = reinterpret_cast<volatile uint8_t*>(port_addr);
        return (*port & (1 << field)) != 0;
    }
};

class avr5_spi : public databus {
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
    void setup(uint32_t speed) override {
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

    void async_read_to(interrupt_i8 *i) override {}

    void enable() override {
        SPCR->SPE = true;
    }

    void disable() override {
        SPCR->SPE = false;
    }

    void write(uint8_t b) override {
        *SPDR = b;
        while (!SPSR->SPIF);
    }

    void write_array(char data[], uint8_t data_size) override {
        uint8_t i = 0u;
        while (i < data_size) {
            *SPDR = (uint8_t)data[i];
            while (!SPSR->SPIF);
            i++;
        }
    }

    uint8_t read() override {
        //TODO: Verify correctness
        while (!SPSR->SPIF);
        return *SPDR;
    }

    bool has_data() override {
        //TODO: Verify correctness
        return SPSR->SPIF;
    }

    bool start_transaction(uint16_t address) override { return true; }
    void end_transaction() override { return; }

    databus_protocol get_protocol() override {
        return databus_protocol::SPI;
    }
};

class avr5_uart0 : public databus {
    mcu *mmcu;
public:
    avr5_uart0(mcu *m) {
        mmcu = m;
    }

    void setup(uint32_t baud) override {
        // use 2x
        UCSR0A->U2X0 = false;
        
        uint8_t multipl = 16u;
        if (UCSR0A->U2X0) {
            multipl = 8u;
        }

        auto div = uint32_t(baud) * multipl;
        auto velocity = (mmcu->clock() / div) - 1;

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

    void async_read_to(interrupt_i8 *i) override {
        //int_usart_rx = i;
        UCSR0B->RXCIE0 = true;
    }

    void enable() override {
        // enable tx and rx
        UCSR0B->RXEN0 = true;
        UCSR0B->TXEN0 = true;
    }

    void disable() override {
        // disable tx and rx
        UCSR0B->RXEN0 = false;
        UCSR0B->TXEN0 = false;
        // disable RX interruption
        UCSR0B->RXCIE0 = false;
    }

    void write(uint8_t b) override {
        while (!UCSR0A->UDRE0);
        *UDR0 = b;
    }

    void write_array(char data[], uint8_t data_size) override {
        uint8_t i = 0u;
        while (i < data_size) {
            write((uint8_t)data[i]);
            i++;
        }
    }

    uint8_t read() override {
        while (!UCSR0A->RXC0);
        return *UDR0;    
    }

    bool has_data() override {
        return UCSR0A->RXC0;
    }

    bool start_transaction(uint16_t address) override { return true; }
    void end_transaction() override { }
    
    databus_protocol get_protocol() override {
        return databus_protocol::UART;
    }
};


class avr5mcu : public mcu {
public:

    uint32_t clock() override { 
        return 16000000;
    }

    virtual void set_interruptions(bool enabled) override {
        if (enabled) {
            asm ("sei");
        } else {
            asm ("cli");
        }
    }

	virtual void wait_ms(uint16_t ms) override {
        uint16_t count = (uint16_t)(clock() / 1280000) * ms - 1;
        busy_wait_loop5(count);
	}

    avr5mcu_port<DDRB, PORTB, 0> b0;
    avr5mcu_port<DDRB, PORTB, 1> b1;
    avr5mcu_port<DDRB, PORTB, 2> b2;
    avr5mcu_port<DDRB, PORTB, 3> b3;
    avr5mcu_port<DDRB, PORTB, 4> b4;
    avr5mcu_port<DDRB, PORTB, 5> b5;

    avr5_spi spi;
    avr5_uart0 uart0;

    avr5mcu(): spi(&b3, &b4, &b5), uart0(this) {
    }
};
