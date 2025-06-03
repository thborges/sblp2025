/*
 * Part of the Robotics Language Standard Library, under the
 * Apache License v2.0.
 */

#pragma once
#include "../cppdeps.h"

enum class displayorientation {NORMAL, NORMAL_MIRRORED, UPSIDE_DOWN, UPSIDE_DOWN_MIRRORED};

template<typename T>
concept display = requires(T obj, displayorientation o, uint8_t u8) {
    { obj.rows() } -> same_as<uint16_t>;
    { obj.columns() } -> same_as<uint16_t>;
    { obj.init_display() } -> same_as<void>;
    { obj.set_orientation(o) } -> same_as<void>;
    { obj.set_contrast(u8) } -> same_as<void>;
    { obj.update_frame() } -> same_as<void>;
    { obj.clear() } -> same_as<void>;
    { obj.set_address(u8) } -> same_as<void>;
};
