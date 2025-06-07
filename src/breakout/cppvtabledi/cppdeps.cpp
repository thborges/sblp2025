
#include "cppdeps.h"

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

    void* memcpy(void* dest, const void* src, size_t n) {
        uint8_t* d = (uint8_t*)dest;
        const uint8_t* s = (const uint8_t*)src;
        while (n--) {
            *d++ = *s++;
        }
        return dest;
    }

    extern uint8_t __heap_start;
    uint8_t* heap_ptr = (uint8_t*)&__heap_start;

}

void* operator new(size_t size) {
    void* result = heap_ptr;
    heap_ptr += size;
    return result;
}

void operator delete(void* ptr, unsigned int) noexcept {
    // no-op or implement free logic
}
