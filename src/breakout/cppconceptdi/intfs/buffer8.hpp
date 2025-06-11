/*
 * Part of the Robotics Language Standard Library, under the
 * Apache License v2.0.
 */
 
#pragma once
#include "../cppdeps.h"
#include "../graphic/envelope.hpp"

class c_buffer8;

template<typename T>
concept buffer8 = not boost::di::aux::is_complete<T>::value or requires(T obj) {
    { obj.height() } -> same_as<uint16_t>;
    { obj.width() } -> same_as<uint16_t>;
    { obj.clear() } -> same_as<void>;
    { obj.get(uint16_t{}, uint16_t{}) } -> same_as<uint8_t>;
    { obj.set(uint16_t{}, uint16_t{}, uint8_t{}) } -> same_as<void>;
    { obj.get_changes() } -> same_as<envelope_u16>;
};
