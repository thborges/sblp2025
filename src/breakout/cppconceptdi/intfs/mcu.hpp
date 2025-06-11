
#pragma once
#include "../cppdeps.h"

class c_mcu;

template<typename T>
concept mcu = not boost::di::aux::is_complete<T>::value or requires(T obj) {
    { obj.wait_ms(uint16_t{}) } -> same_as<void>;
    { obj.set_interruptions(bool{}) } -> same_as<void>;
    { obj.clock() } -> same_as<uint32_t>;
};
