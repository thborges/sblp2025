
#include "math.hpp"

uint16_t random_seed16 = 0xE7A8;

int8_t abs8(int8_t value) {
    uint8_t mask = value >> 7;
    value = (value ^ mask) - mask;
    return value;
}

void srandom16(uint16_t seed) {
    random_seed16 = seed;
}

uint16_t random16() {
    random_seed16 += 0xfc15;
    uint32_t n = (uint32_t)random_seed16 * 0x2ab;
    return (uint16_t)((n >> 16) ^ n);
}
