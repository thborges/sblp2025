
#pragma once
#include "../cppdeps.h"

template<typename T>
concept mcu = requires(T obj, uint16_t u16, bool b) {
    { obj.wait_ms(u16) } -> same_as<void>;
    { obj.set_interruptions(b) } -> same_as<void>;
    { obj.clock() } -> same_as<uint32_t>;
};
