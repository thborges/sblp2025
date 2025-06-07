/*
 * Robcmp examples: Breakout game on a SSD1306 display
 */

#include "intfs/ports.hpp"
#include "intfs/mcu.hpp"
#include "intfs/display.hpp"
#include "intfs/databus.hpp"

#include "mcu/avr5_hardware.hpp"
//#include "other_hardware.hpp"

#include "game.hpp"

template<databus uart, databus displaydb, display dis, mcu mcu>
void setup_hardware(mcu& mmcu, uart& dbus_uart, displaydb& dbus_display,
    dis& oled) {

    // dbus_serial setup (enabled at dbus_uart creation)
    const char *msg1 = "Robcmp Breakout Game for SSD1306 肖\n";
    const char *msg2 = "Use left/right arrows to move and space to pause.\n";
    dbus_uart.write_array((char*)msg1, strlen(msg1));
    dbus_uart.write_array((char*)msg2, strlen(msg2));

    // dbus_display setup
    dbus_display.setup(0);
    dbus_display.enable();

    // init display
    oled.set_address(0x78);
    oled.init_display();
    oled.clear();

    // enable interruptions
    mmcu.set_interruptions(true);
}

int main() {

    avr5mcu mmcu;
    avr5_uart0 dbus_uart(mmcu);

    avr5mcu_b0 b0;
    avr5mcu_b1 b1;
    avr5mcu_b2 b2;
    avr5mcu_b3 b3;
    avr5mcu_b4 b4;
    avr5mcu_b5 b5;
    avr5_spi spi(b3, b4, b5, b2);
    ssd1306_framebuffer framebuffer;
    ssd1306 oled(mmcu, b1, b0, b2, spi, framebuffer);
    canvas8 canvas(framebuffer);

    setup_hardware(mmcu, dbus_uart, spi, oled);

    game gm(canvas, oled, mmcu);
    //k = gm.key_press;
    //dbus_uart.async_read_to(k);

    while (true) {
        gm.process_game();
        oled.update_frame();

        if (gm.get_bar_width() == 10) {
            dbus_uart.write('$');
            return 0;
        }
    }
}
