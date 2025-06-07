/*
 * Part of the Robotics Language Standard Library, under the
 * Apache License v2.0.
*/

#pragma once
#include "../cppdeps.h"
#include "../intfs/buffer8.hpp"

template<buffer8 buffer8>
class canvas8 {
    buffer8& buffer;
public:
    canvas8(buffer8& buf) : buffer(buf) {
    }
    
    uint16_t width() {
        return buffer.width();
    }

    uint16_t height() {
        return buffer.height()*8;
    }

    void clear() {
        buffer.clear();
    }

    void set_pixel(uint16_t row, uint16_t col) {
        auto rowb = row/8;
        assert(col < buffer.width() && rowb < buffer.height() && "outside buffer bounds");
        auto v = buffer.get(rowb, col);
        buffer.set(rowb, col, v | (1 << uint8_t(row & 7)));
    }

    void clear_pixel(uint16_t row, uint16_t col) {
        auto rowb = row/8;
        assert(col < buffer.width() && rowb < buffer.height() && "outside buffer bounds");
        auto v = buffer.get(rowb, col);
        buffer.set(rowb, col, v & ~(1 << uint8_t(row & 7)));
    }

    void fill_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, bool v) {
        if (x1 >= width()) { return; }
        if (y1 >= height()) { return; }
        if (x2 >= width()) { x2 = width()-1; }
        if (y2 >= height()) { y2 = height()-1; }
        auto first_bank = y1 >> 3;
        auto last_bank = y2 >> 3;
        auto bank = first_bank;
        while (bank <= last_bank) {
            auto mask = 0xFF;
            auto y1t = uint8_t(y1);
            auto y2t = uint8_t(y2);
            if (first_bank == last_bank) { // all bits in the same bank
                mask = (mask >> ((y1t & 7) + 7 - (y2t & 7))) << (y1t & 7);
            } else if (bank == first_bank) {
                mask = (mask << (y1t & 7));
            } else if (bank == last_bank) {
                mask = (mask >> (7 - (y2t & 7)));
            }
            auto b = x1;
            while (b <= x2) {
                auto bv = buffer.get(bank, b);
                bv &= ~mask;
                if (v) {
                    bv |= mask;
                }
                buffer.set(bank, b, bv);
                b++;
            }
            bank++;
        }
    }
};
