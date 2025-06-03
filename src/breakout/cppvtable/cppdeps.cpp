
#include <stdlib.h>
#include <stdint.h>

extern "C" {
    void __cxa_pure_virtual() {}

    size_t strlen(const char* s) {
        const char* p = s;
        while (*p) ++p;
        return (size_t)(p - s);
    }

    void* memcpy(void* dest, const void* src, size_t n) {
        uint8_t* d = (uint8_t*)dest;
        const uint8_t* s = (const uint8_t*)src;
        while (n--) {
            *d++ = *s++;
        }
        return dest;
    }
}
