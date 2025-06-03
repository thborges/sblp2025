
#pragma once
#include <stdint.h>

template <typename mcu_impl>
struct mcu {
    virtual void wait_ms(uint16_t ms) = 0;
    virtual void set_interruptions(bool enabled) = 0;
    
    /* clock is meant to return a constant, but constexpr functions
       can't be virtual: that prevents relevant optimizations.
       We use a CRTP metaprograming pattern here. */
    constexpr uint32_t clock() {
        return static_cast<const mcu_impl*>(this)->clock_impl();
    }
};

enum class portmode {input = 0, output = 1};

struct digitalport {
    virtual void mode(portmode m) = 0;
    virtual void set(bool v) = 0;
    virtual bool get() = 0;
};

struct analogport {
    virtual void mode(portmode m) = 0;
    virtual void set(uint16_t v) = 0;
    virtual uint16_t get() = 0;
};

