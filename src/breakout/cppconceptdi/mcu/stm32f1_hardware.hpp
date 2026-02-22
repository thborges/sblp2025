
#pragma once

#include "../graphic/ssd1306.hpp"
#include "../graphic/canvas8.hpp"
#include "stm32f1_mcu.hpp"

class c_databus_uart0;
class c_databus_display;

class c_digitalport_datacmd;
class c_digitalport_reset;
class c_digitalport_select;

using namespace boost::di;

// injections

using display_t = ssd1306<stm32f1_spi1, stm32f1_port_b7, stm32f1_port_b6, stm32f1_port_b8, stm32f1, ssd1306_framebuffer>;

auto breakout_injector = make_injector(
    bind<c_mcu>.to<stm32f1>(),
    bind<c_databus_uart0>.to<stm32f1_uart2>(),
    bind<c_buffer8>.to<ssd1306_framebuffer>(),
    bind<c_databus_display>.to<stm32f1_spi1>(),
    bind<c_display>.to<display_t>(),
    
    //display ports need by ssd1306
    bind<c_digitalport_datacmd>.to<stm32f1_port_b7>(),
    bind<c_digitalport_reset>.to<stm32f1_port_b6>(),
    bind<c_digitalport_select>.to<stm32f1_port_b8>()
);
