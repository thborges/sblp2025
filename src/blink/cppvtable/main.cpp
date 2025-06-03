
#include "intfs.hpp"

#include "avr5_hardware.hpp"
//#include "other_hardware.hpp"

int main() {
    led.mode(portmode::output);
    while (true) {
        led.set(true);
        mmcu.wait_ms(100);
        led.set(false);
        mmcu.wait_ms(100);
    }
}
