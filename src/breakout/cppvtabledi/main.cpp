/*
 * Robcmp examples: Breakout game on a SSD1306 display
 */

#include "cppdeps.h"
#include "intfs/ports.hpp"
#include "intfs/mcu.hpp"
#include "intfs/display.hpp"
#include "intfs/databus.hpp"

#include "mcu/avr5_hardware.hpp"
//#include "other_hardware.hpp"

#include "game.hpp"

class app {
public:
    mcu& mmcu; 
    display& oled;
    databus& dbus_uart;
    databus& dbus_display;

    BOOST_DI_INJECT(app,
        mcu& mmcu, 
        (named = named_dbus_uart) databus& dbus_uart, 
        (named = named_dbus_display) databus& dbus_display,
        display& oled) : 
        mmcu(mmcu), oled(oled), 
        dbus_uart(dbus_uart), dbus_display(dbus_display) {}

    void setup_hardware() {
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
};

int main() {

    auto mapp = injector.create<app>();
    mapp.setup_hardware();

    auto canvas = injector.create<canvas8>();
    
    auto &dbus_uart = mapp.dbus_uart;
    auto &mmcu = mapp.mmcu;
    auto &oled = mapp.oled;

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
