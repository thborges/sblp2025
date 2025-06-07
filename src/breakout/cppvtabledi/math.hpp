
#pragma once
#include "cppdeps.h"

const uint16_t uint16_max = 65535u;
extern uint16_t random_seed16;

int8_t abs8(int8_t value);
void srandom16(uint16_t seed);
uint16_t random16();
 