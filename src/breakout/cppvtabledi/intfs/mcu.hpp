
#pragma once
#include "../../../common/cppdeps.h"

struct mcu {
    //virtual ~mcu() noexcept = default;
    virtual void wait_ms(uint16_t ms) = 0;
    virtual void set_interruptions(bool enabled) = 0;
    virtual uint32_t clock() = 0;
};
