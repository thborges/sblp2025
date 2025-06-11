
#pragma once

#include "../graphic/ssd1306.hpp"
#include "../graphic/canvas8.hpp"
#include "avr5_mcu.cpp"
#include "../di.hpp"
namespace di = boost::di;

const auto nm_dbus_uart = [] {};

using namespace di;

// injections
auto breakout_injector = make_injector(
    bind<mcu>.to<avr5mcu>(),
    bind<display>.to<ssd1306>(),
    bind<buffer8>.to<ssd1306_framebuffer>(),
    bind<databus>.named(nm_dbus_uart).to<avr5_uart0>(),
    bind<databus>.named(nm_dbus_display).to<avr5_spi>(),

    //display ports need by ssd1306
    bind<digitalport>.named(dp_datacmd).to<avr5mcu_b1>(),
    bind<digitalport>.named(dp_reset).to<avr5mcu_b0>(),
    bind<digitalport>.named(dp_select).to<avr5mcu_b2>(),

    //SPI ports need by avr5_spi
    bind<digitalport>.named(avr5_mosi).to<avr5mcu_b3>(),
    bind<digitalport>.named(avr5_miso).to<avr5mcu_b4>(),
    bind<digitalport>.named(avr5_sck).to<avr5mcu_b5>(),
    bind<digitalport>.named(avr5_ss).to<avr5mcu_b2>()
);
