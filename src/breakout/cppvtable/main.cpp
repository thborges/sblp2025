/*
 * Robcmp examples: Breakout game on a SSD1306 display
 */

#include <string.h>
#include "intfs/ports.hpp"
#include "intfs/mcu.hpp"
#include "intfs/display.hpp"
#include "intfs/databus.hpp"

#include "mcu/avr5_hardware.hpp"
//#include "other_hardware.hpp"

#include "game.hpp"

void setup_hardware(mcu *mmcu, databus *dbus_uart, databus *dbus_display,
    display *oled) {

    // dbus_serial setup
    dbus_uart->setup(115200);
    dbus_uart->enable();
    const char *msg1 = "Robcmp Breakout Game for SSD1306 肖\n";
    const char *msg2 = "Use left/right arrows to move and space to pause.\n";
    dbus_uart->write_array((char*)msg1, strlen(msg1));
    dbus_uart->write_array((char*)msg2, strlen(msg2));

    // dbus_display setup
    dbus_display->setup(0);
    dbus_display->enable();

	// init display
	oled->set_address(0x78);
	oled->init_display();
	//oled.clear();

    // enable interruptions
    mmcu->set_interruptions(true);
}

int main() {

    avr5mcu mmcu;
    ssd1306 oled(&mmcu, &mmcu.b1, &mmcu.b0, &mmcu.b2, &mmcu.spi);
    databus *dbus_uart = &mmcu.uart0;
    databus *dbus_display = &mmcu.spi;
    canvas8 canvas(&oled.framebuffer);

    setup_hardware(&mmcu, dbus_uart, dbus_display, &oled);

    game gm;
    //k = gm.key_press;
    //dbus_uart.async_read_to(k);

    while (true) {
        gm.move_bar(canvas);

        if (!gm.paused && !gm.gameover) {
            //mmcu.wait_ms(20);
            gm.move_ball(canvas);
        }

        gm.check_level_done(canvas, &oled, &mmcu);
        oled.update_frame();

        dbus_uart->write('.');
        if (gm.bar_width == 10) {
            return 0;
        }
    }
}
