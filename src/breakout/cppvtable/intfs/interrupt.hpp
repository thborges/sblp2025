/*
 * Part of the Robotics Language Standard Library, under the
 * Apache License v2.0.
 */

#pragma once
#include <stdint.h>

struct interrupt {
    virtual void handler() = 0;
};

struct interrupt_i8 {
    virtual void handler(uint8_t i) = 0;
};

class void_interrupt : interrupt {
    void handler() {}
};

class void_interrupt_i8 : interrupt_i8 {
    void handler(uint8_t i) {}
};
