
#include "../../../common/cppdeps.h"
#include "../intfs/mcu.hpp"
#include "../intfs/ports.hpp"
#include "../intfs/databus.hpp"
#include "../../../common/stm32f1_regs.hpp"


class avr5mcu_port_b6 : public digitalport {
private:
    void mode_input(PIN_MODE mode, INPUT_MODE cnf) {
        GPIOB_CRL->mode6 = mode;
        GPIOB_CRL->cnf6 = cnf;
    }
    void mode_output(PIN_MODE mode, OUTPUT_MODE cnf) {
        GPIOB_CRL->mode6 = mode;
        GPIOB_CRL->cnf6 = cnf;
    }

public:
    void mode(port_mode m)  {
        APB2EN->iopb = true;
        if (m == port_mode::input) {
            mode_input(PIN_MODE::INPUT, INPUT_MODE::PULLUPDOWN);
        } else {
            mode_output(PIN_MODE::OUTPUT50MHz, OUTPUT_MODE::PUSHPULL);
        }
    }
    void set(bool v) { GPIOB_ODR->odr6 = v; }
    bool get()       { return GPIOB_IDR->idr6; }
};


class avr5mcu_port_b7 : public digitalport {
private:
    void mode_input(PIN_MODE mode, INPUT_MODE cnf) {
        GPIOB_CRL->mode7 = mode;
        GPIOB_CRL->cnf7 = cnf;
    }
    void mode_output(PIN_MODE mode, OUTPUT_MODE cnf) {
        GPIOB_CRL->mode7 = mode;
        GPIOB_CRL->cnf7 = cnf;
    }

public:
    void mode(port_mode m)  {
        APB2EN->iopb = true;
        if (m == port_mode::input) {
            mode_input(PIN_MODE::INPUT, INPUT_MODE::PULLUPDOWN);
        } else {
            mode_output(PIN_MODE::OUTPUT50MHz, OUTPUT_MODE::PUSHPULL);
        }
    }
    void set(bool v) { GPIOB_ODR->odr7 = v; }
    bool get()       { return GPIOB_IDR->idr7; }
};


class avr5mcu_port_b8 : public digitalport {
private:
    void mode_input(PIN_MODE mode, INPUT_MODE cnf) {
        GPIOB_CRH->mode8 = mode;
        GPIOB_CRH->cnf8 = cnf;
    }
    void mode_output(PIN_MODE mode, OUTPUT_MODE cnf) {
        GPIOB_CRH->mode8 = mode;
        GPIOB_CRH->cnf8 = cnf;
    }

public:
    void mode(port_mode m)  {
        APB2EN->iopb = true;
        if (m == port_mode::input) {
            mode_input(PIN_MODE::INPUT, INPUT_MODE::PULLUPDOWN);
        } else {
            mode_output(PIN_MODE::OUTPUT50MHz, OUTPUT_MODE::PUSHPULL);
        }
    }
    void set(bool v) { GPIOB_ODR->odr8 = v; }
    bool get()       { return GPIOB_IDR->idr8; }
};


class stm32f1 : public mcu {
public:
	
	uint32_t clock() override {
		// TODO: provide 25Mhz and 72Mhz selection
		return 8E6;
	}

	void set_interruptions(bool enabled) override {
        return;
	}

    void wait_us(uint16_t us) {
		uint32_t ticks = clock()/1E6 * us;
    	uint32_t start_tick = *SYSTICK_VAL;
    	while (start_tick - *SYSTICK_VAL < ticks);
	}

	void wait_ms(uint16_t ms) override {
		while (ms > 0) {
			wait_us(1000);
			ms--;
		}
	}

	uint32_t micros() {
		return *SYSTICK_VAL / (clock()/1E6);
	}

	uint32_t millis() {
		return micros() / 1000;
	}

};

class stm32f1_spi1 : public databus {
public:
    stm32f1_spi1() { }

    void setup(uint32_t speed) override {
        //APB2EN->afio = true; // enable alternate function I/O clock
        APB2EN->spi1 = true; // enable SPI1

        SPI1_CR1->SPE = false; // disable SPI first
        SPI1_CR1->CPHA = true;
        SPI1_CR1->CPOL = true;
        SPI1_CR1->MSTR = true;    // Master mode
        SPI1_CR1->LSBFIRST = false; // MSB first
        SPI1_CR1->RXONLY = false;
        SPI1_CR1->DFF = false; // 8 bit frame
        SPI1_CR1->SSM = true;
        SPI1_CR1->SSI = true;

        // speed
        SPI1_CR1->BR = 1u; // Pclk / 2 = 4 MHz
        
        APB2EN->iopa = true;
        APB2EN->iopb = true;

        // set port a5, SCK
        GPIOA_CRL->mode5 = PIN_MODE::OUTPUT50MHz;
        GPIOA_CRL->cnf5 = OUTPUT_MODE::APUSH_PULL;

        // set port a7, MOSI
        GPIOA_CRL->mode7 = PIN_MODE::OUTPUT50MHz;
        GPIOA_CRL->cnf7 = OUTPUT_MODE::APUSH_PULL;

        // set port a6, MISO
        GPIOA_CRL->mode6 = PIN_MODE::INPUT;
        GPIOA_CRL->cnf6 = INPUT_MODE::FLOATING;
    }

    void enable() override { SPI1_CR1->SPE = true; }

    void disable() override { SPI1_CR1->SPE = false;}
    
    void write(uint8_t b) override {
        while (!SPI1_SR->TXE); // wait transmit buffer empty
        SPI1_DR->DR = b;
        while (SPI1_SR->BSY); // wait transfer finish
    }

    void write_array(char data[], uint8_t data_size) override {
        uint8_t i = 0u;
        while (i < data_size) {
            while (!SPI1_SR->TXE);
            SPI1_DR->DR = data[i];
            i++;
        }
        while (SPI1_SR->BSY); // wait transfer finish
    }

    uint8_t read() override { return 0; }

    bool has_data() override { return false; }

    bool start_transaction(uint16_t address) override { return true; }
    void end_transaction() override { }
    void async_read_to(interrupt_i8 *i) override { }

    databus_protocol get_protocol() override {
        return databus_protocol::SPI;
    }
};


class stm32f1_uart2 : public databus {
    mcu& mmcu;
public:
    stm32f1_uart2(mcu& mmcu) : mmcu(mmcu) {
        // send ^ to start measuring time in experiments
        setup(115200);
        enable();
        write('^');
    }

    void setup(uint32_t baud) override {
        APB1EN->usart2 = true; // enable usart2 clock
        //AHBENR->sdio = true; // enable SDIO clock
        APB2EN->afio = true; //? enable alternate function I/O clock
        APB2EN->iopa = true; // port A clock

        // set port a2 (output TX) 
        GPIOA_CRL->mode2 = PIN_MODE::OUTPUT2MHz;
        GPIOA_CRL->cnf2 = OUTPUT_MODE::APUSH_PULL;

        // set port a3 (input RX)
        GPIOA_CRL->mode3 = PIN_MODE::INPUT;
        GPIOA_CRL->cnf3 = INPUT_MODE::FLOATING;

        // assuming reset configuration, that is 8 bits per byte,
        // no parity checks, and 1 “stop bit”. 

        // set baudrate
        auto uartdiv = mmcu.clock() / baud;
        USART2_BRR->DIV_Fraction = uartdiv % 16;
        USART2_BRR->DIV_Mantissa = uartdiv / 16;
    }

    void enable() override {
        USART2_CR1->RE = true; // receptor
        USART2_CR1->TE = true; // transmiter
        USART2_CR1->UE = true; // usart enable
    }

    void disable() override {
        USART2_CR1->UE = false;
    }

    void write(uint8_t b) override {
        while (!USART2_SR->TXE);
        USART2_DR->DR = b;
    }
    
    void write_array(char data[], uint8_t data_size) override {
        uint8_t i = 0u;
        while (i < data_size) {
            write(uint8_t(data[i]));
            i++;
        }
    }
    
    uint8_t read() override { 
        while (!USART2_SR->RXNE);
        return uint8_t(USART2_DR->DR);
    }

    bool has_data() override {
        return USART2_SR->RXNE;
    }

    bool start_transaction(uint16_t address) override { return true; }
    void end_transaction() override {}
    
    virtual void async_read_to(interrupt_i8 *i) override { }

    databus_protocol get_protocol() override {
        return databus_protocol::UART;
    }
};
