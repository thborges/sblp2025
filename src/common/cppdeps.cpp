
#include "../../common/cppdeps.h"

extern "C" {
    void __cxa_pure_virtual() {}
    int __cxa_guard_acquire(uint64_t* g) { return !*g; }
    void __cxa_guard_release(uint64_t* g) { *g = 1; }
    void __cxa_guard_abort(uint64_t*) {}
    
    int atexit(void (*func)(void)) { return 0; }
    
    size_t strlen(const char* s) {
        const char* p = s;
        while (*p) ++p;
        return (size_t)(p - s);
    }

    void* __attribute__((weak)) memcpy(void* dest, const void* src, size_t n) {
        uint8_t* d = (uint8_t*)dest;
        const uint8_t* s = (const uint8_t*)src;
        while (n--) {
            *d++ = *s++;
        }
        return dest;
    }

    void __aeabi_memcpy(void *dest, const void *src, size_t n) {
        unsigned char *d = (unsigned char *)dest;
        const unsigned char *s = (const unsigned char *)src;
        while (n--) {
            *d++ = *s++;
        }
    }

    void __aeabi_memclr(void* dest, size_t n) {
        unsigned char* d = (unsigned char*)dest;
        while (n--) {
            *d++ = 0;
        }
    }
}
