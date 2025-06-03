
#pragma once

typedef char int8_t;
typedef unsigned char uint8_t;

typedef int int16_t;
typedef unsigned int uint16_t;
typedef unsigned int size_t;

typedef long int int32_t;
typedef unsigned long int uint32_t;

typedef unsigned int uintptr_t;

#define assert(cond) ((void)0)
#define NULL nullptr

extern "C" {
    size_t strlen(const char* s);
};

struct false_type {
    static constexpr bool value = false;
    using type = false_type;
    constexpr operator bool() const noexcept { return value; }
};

struct true_type {
    static constexpr bool value = true;
    using type = true_type;
    constexpr operator bool() const noexcept { return value; }
};

template<typename T, typename U>
struct is_same : false_type {};

template<typename T>
struct is_same<T, T> : true_type {};

template<typename T, typename U>
inline constexpr bool is_same_v = is_same<T, U>::value;

template<class T, class U>
concept same_as = is_same_v<T, U> &&
                  is_same_v<U, T>;
