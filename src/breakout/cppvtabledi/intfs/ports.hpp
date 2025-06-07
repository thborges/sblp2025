
#pragma once
#include "../cppdeps.h"

enum class port_mode {input = 0, output = 1};

struct digitalport {
    //virtual ~digitalport() noexcept = default;
    virtual void mode(port_mode m) = 0;
    virtual void set(bool v) = 0;
    virtual bool get() = 0;
};

struct analogport {
    virtual void mode(port_mode m) = 0;
    virtual void set(uint16_t v) = 0;
    virtual uint16_t get() = 0;
};
