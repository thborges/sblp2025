/*
 * Part of the Robotics Language Standard Library, under the
 * Apache License v2.0.
 */

#pragma once
#include "../cppdeps.h"

template<typename T>
concept interrupt = requires(T obj) {
    { obj.handler() } -> same_as<void>;
};

template<typename T>
concept interrupt_i8 = requires(T obj, uint8_t i) {
    { obj.handler(i) } -> same_as<void>;
};

class void_interrupt {
    void handler() {}
};

class void_interrupt_i8 {
    void handler(uint8_t i) {}
};
