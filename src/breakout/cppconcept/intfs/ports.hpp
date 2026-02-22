
#pragma once
#include "../../../common/cppdeps.h"

enum class port_mode {input = 0, output = 1};

template<typename T>
concept digitalport = requires(T obj) {
    { obj.mode(port_mode{}) } -> same_as<void>;
    { obj.set(bool{}) } -> same_as<void>;
    { obj.get() } -> same_as<bool>;
};
