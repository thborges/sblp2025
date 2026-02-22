/*
 * Robcmp examples: Breakout game on a SSD1306 display
 */

#include "intfs/ports.hpp"
#include "intfs/mcu.hpp"
#include "intfs/display.hpp"
#include "intfs/databus.hpp"

#define STM32 1

#ifdef AVR
#include "mcu/avr5_hardware.hpp"
#elif STM32
#include "mcu/stm32f1_hardware.hpp"
#endif

#include "game.hpp"

template<
    mcu mcu_impl = c_mcu,
    display display_impl = c_display,
    databus dbus_uart_impl = c_databus_uart0,
    databus dbus_display_impl = c_databus_display>
class app {
public:
    mcu_impl& mmcu; 
    display_impl& oled;
    dbus_uart_impl& dbus_uart;
    dbus_display_impl& dbus_display;

    app(mcu_impl& mmcu, dbus_uart_impl& dbus_uart, dbus_display_impl& dbus_display,
        display_impl& oled) : 
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
    auto mapp = breakout_injector.create<app>();
    mapp.setup_hardware();

    auto canvas = breakout_injector.create<canvas8>();

    game gm(canvas, mapp.oled, mapp.mmcu);
    //k = gm.key_press;
    //dbus_uart.async_read_to(k);

    while (true) {
        gm.process_game();
        mapp.oled.update_frame();
        mapp.dbus_uart.write('-');

        if (gm.get_bar_width() == 10) {
            mapp.dbus_uart.write('$');
            return 0;
        }
    }
}
