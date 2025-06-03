
#include "intfs.hpp"
#include "avr5_regs.hpp"

extern "C" __attribute((naked)) void __delay_us();
extern "C" void busy_wait_loop5(uint16_t count);

template<uintptr_t ddr_addr, uintptr_t port_addr, uint8_t field>
class avr5mcu_port : public digitalport {
public:
    virtual void mode(portmode m) override {
        volatile uint8_t* ddr = reinterpret_cast<volatile uint8_t*>(ddr_addr);
        *ddr &= ~(1 << field);
        if (m == portmode::output)
            *ddr |= (1 << field);
    }

    virtual void set(bool value) override {
        volatile uint8_t* port = reinterpret_cast<volatile uint8_t*>(port_addr);
        if (value)
            *port |= (1 << field);
        else
            *port &= ~(1 << field);
    }

    virtual bool get() override {
        volatile uint8_t* port = reinterpret_cast<volatile uint8_t*>(port_addr);
        return (*port & (1 << field)) != 0;
    }
};

class avr5mcu : public mcu<avr5mcu> {
public:

    constexpr uint32_t clock_impl() const { 
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
    avr5mcu_port<DDRB, PORTB, 5> b5;
};
