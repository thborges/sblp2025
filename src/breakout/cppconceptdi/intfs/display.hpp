/*
 * Part of the Robotics Language Standard Library, under the
 * Apache License v2.0.
 */

#pragma once
#include "../../../common/cppdeps.h"

class c_display;

enum class displayorientation {NORMAL, NORMAL_MIRRORED, UPSIDE_DOWN, UPSIDE_DOWN_MIRRORED};

template<typename T>
concept display = not boost::di::aux::is_complete<T>::value or requires(T obj) {
    { obj.rows() } -> same_as<uint16_t>;
    { obj.columns() } -> same_as<uint16_t>;
    { obj.init_display() } -> same_as<void>;
    { obj.set_orientation(displayorientation{}) } -> same_as<void>;
    { obj.set_contrast(uint8_t{}) } -> same_as<void>;
    { obj.update_frame() } -> same_as<void>;
    { obj.clear() } -> same_as<void>;
    { obj.set_address(uint8_t{}) } -> same_as<void>;
};
