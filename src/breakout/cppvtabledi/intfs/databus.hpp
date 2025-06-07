/*
 * Part of the Robotics Language Standard Library, under the
 * Apache License v2.0.
 */

#pragma once
#include "../cppdeps.h"
#include "interrupt.hpp"

enum class databus_protocol {SPI, I2C, UART, CAN};

struct databus {
    //virtual ~databus() noexcept = default;
    virtual void setup(uint32_t speed) = 0;
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual void write(uint8_t b) = 0;
    virtual void write_array(char data[], uint8_t data_size) = 0;
    virtual uint8_t read() = 0;
    virtual bool has_data() = 0;
    virtual bool start_transaction(uint16_t addr) = 0;
    virtual void end_transaction() = 0;
    virtual databus_protocol get_protocol() = 0;
    virtual void async_read_to(interrupt_i8 *i) = 0;
};
 