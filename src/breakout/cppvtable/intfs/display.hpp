/*
 * Part of the Robotics Language Standard Library, under the
 * Apache License v2.0.
 */

#pragma once
#include "../../../common/cppdeps.h"

enum class displayorientation {NORMAL, NORMAL_MIRRORED, UPSIDE_DOWN, UPSIDE_DOWN_MIRRORED};

struct display {
    virtual uint16_t rows() = 0;
    virtual uint16_t columns() = 0;
    virtual void init_display() = 0;
    virtual void set_orientation(displayorientation o) = 0;
    virtual void set_contrast(uint8_t c) = 0;
    virtual void update_frame() = 0;
    virtual void clear() = 0;
    virtual void set_address(uint8_t a) = 0;
};
