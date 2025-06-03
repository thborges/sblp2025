
#pragma once
#include "../cppdeps.h"

enum class port_mode {input = 0, output = 1};

template<typename T>
concept digitalport = requires(T obj, port_mode m, bool v) {
    { obj.mode(m) } -> same_as<void>;
    { obj.set(v) } -> same_as<void>;
    { obj.get() } -> same_as<bool>;
};
