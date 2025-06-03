/*
 * Part of the Robotics Language Standard Library, under the
 * Apache License v2.0.
 */
 
#pragma once
#include "../cppdeps.h"
#include "../graphic/envelope.hpp"

template<typename T>
concept buffer8 = requires(T obj, uint16_t row, uint16_t col, uint8_t v) {
    { obj.height() } -> same_as<uint16_t>;
    { obj.width() } -> same_as<uint16_t>;
    { obj.clear() } -> same_as<void>;
    { obj.get(row, col) } -> same_as<uint8_t>;
    { obj.set(row, col, v) } -> same_as<void>;
    { obj.get_changes() } -> same_as<envelope_u16>;
};
