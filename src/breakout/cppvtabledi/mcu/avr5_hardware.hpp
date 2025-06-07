
#pragma once

#include "../graphic/ssd1306.hpp"
#include "../graphic/canvas8.hpp"
#include "avr5_mcu.cpp"
#include "../di.hpp"
namespace di = boost::di;

const auto named_dbus_uart = [] {};

// injections
auto injector = di::make_injector(
    // generic inject, specified later (boost::di shows errors without them)
    di::bind<mcu>.to<avr5mcu>(),
    di::bind<databus>.named(named_dbus_uart).to<avr5_uart0>(),
    di::bind<databus>.named(named_dbus_display).to<avr5_spi>(),

    //display ports need by ssd1306
    di::bind<digitalport>.named(digitalport_datacmd).to<avr5mcu_b1>(),
    di::bind<digitalport>.named(digitalport_reset).to<avr5mcu_b0>(),
    di::bind<digitalport>.named(digitalport_select).to<avr5mcu_b2>(),

    //SPI ports need by avr5_spi
    di::bind<digitalport>.named(avr5_mosi).to<avr5mcu_b3>(),
    di::bind<digitalport>.named(avr5_miso).to<avr5mcu_b4>(),
    di::bind<digitalport>.named(avr5_sck).to<avr5mcu_b5>(),
    di::bind<digitalport>.named(avr5_ss).to<avr5mcu_b2>(),

    di::bind<display>.to<ssd1306>(),
    di::bind<buffer8>.to<ssd1306_framebuffer>()
);
