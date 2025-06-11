

#pragma once
#include "../math.hpp"

class envelope_u16 {
public:
    uint16_t lx = uint16_max;
    uint16_t ly = uint16_max;
    uint16_t ux = 0;
    uint16_t uy = 0;

    void set(uint16_t lxx, uint16_t lyy, uint16_t uxx, uint16_t uyy) {
        lx = lxx;
        ly = lyy;
        ux = uxx;
        uy = uyy;        
    }

    bool empty() {
        return ux < lx || uy < ly;
    }

    void clear() {
        lx = uint16_max;
        ly = uint16_max;
        ux = 0;
        uy = 0;
    }

    void expand(uint16_t x, uint16_t y) {
        if (lx > x) { lx = x; }
        if (ux < x) { ux = x; }
        if (ly > y) { ly = y; }
        if (uy < y) { uy = y; }
    }

};
