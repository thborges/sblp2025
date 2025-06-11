
#pragma once

#include "../di.hpp"
namespace di = boost::di;

#include "../graphic/ssd1306.hpp"
#include "../graphic/canvas8.hpp"
#include "avr5_mcu.hpp"

class c_databus_uart0;
class c_databus_display;

class c_digitalport_b0;
class c_digitalport_b1;
class c_digitalport_b2;
class c_digitalport_b3;
class c_digitalport_b4;
class c_digitalport_b5;

class c_avr5_mosi;
class c_avr5_miso;
class c_avr5_sck;
class c_avr5_ss;

using namespace di;

// injections

using spi_t = avr5_spi<avr5mcu_b3, avr5mcu_b4, avr5mcu_b5, avr5mcu_b2>;
using display_t = ssd1306<spi_t, avr5mcu_b1, avr5mcu_b0, avr5mcu_b2, avr5mcu, ssd1306_framebuffer>;

auto breakout_injector = make_injector(
    bind<c_mcu>.to<avr5mcu>(),
    bind<c_databus_uart0>.to<avr5_uart0>(),
    bind<c_buffer8>.to<ssd1306_framebuffer>(),
    bind<c_databus_display>.to<spi_t>(),
    bind<c_display>.to<display_t>(),
    
    //display ports need by ssd1306
    bind<c_digitalport_b1>.to<avr5mcu_b1>(),
    bind<c_digitalport_b0>.to<avr5mcu_b0>(),
    bind<c_digitalport_b2>.to<avr5mcu_b2>(),

    //SPI ports need by avr5_spi
    bind<c_avr5_mosi>.to<avr5mcu_b3>(),
    bind<c_avr5_miso>.to<avr5mcu_b4>(),
    bind<c_avr5_sck>.to<avr5mcu_b5>(),
    bind<c_avr5_ss>.to<avr5mcu_b2>()
);
