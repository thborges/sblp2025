/*
 * Part of the Robotics Language Standard Library, under the
 * Apache License v2.0.
 */

#pragma once
#include "../cppdeps.h"
#include "interrupt.hpp"

enum class databus_protocol {SPI, I2C, UART, CAN};

template<typename T>
T&& declval() noexcept;

template<typename T>
concept databus = requires(T obj, uint32_t speed, uint8_t u8, uint16_t u16, char data[], void *isr) {
    { obj.setup(speed) } -> same_as<void>;
    { obj.enable() } -> same_as<void>;
    { obj.disable() } -> same_as<void>;
    { obj.write(u8) } -> same_as<void>;
    { obj.write_array(data, u8) } -> same_as<void>;
    { obj.read() } -> same_as<uint8_t>;
    { obj.has_data() } -> same_as<bool>;
    { obj.start_transaction(u16) } -> same_as<bool>;
    { obj.end_transaction() } -> same_as<void>;
    { obj.get_protocol() } -> same_as<databus_protocol>;
    { obj.async_read_to(isr) } -> same_as<void>;
};

