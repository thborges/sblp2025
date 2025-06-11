
#pragma once
#include "../cppdeps.h"
#include "../di.hpp"

class c_digitalport;

enum class port_mode {input = 0, output = 1};

template<typename T>
concept digitalport = not boost::di::aux::is_complete<T>::value or requires(T obj) {
    { obj.mode(port_mode{}) } -> same_as<void>;
    { obj.set(bool{}) } -> same_as<void>;
    { obj.get() } -> same_as<bool>;
};
