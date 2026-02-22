/*
 * Part of the Robotics Language Standard Library, under the
 * Apache License v2.0.
 */
 
#pragma once
#include "../../../common/cppdeps.h"
#include "../graphic/envelope.hpp"

struct buffer8 {
    //virtual ~buffer8() noexcept = default;
    virtual uint16_t height() = 0;
    virtual uint16_t width() = 0;
    virtual void clear() = 0;
    virtual uint8_t get(uint16_t row, uint16_t col) = 0;
    virtual void set(uint16_t row, uint16_t col, uint8_t v) = 0;
    virtual envelope_u16 get_changes() = 0;
    virtual void reset_changes() = 0;
};
