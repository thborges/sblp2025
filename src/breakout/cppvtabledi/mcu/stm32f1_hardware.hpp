
#pragma once

#include "../graphic/ssd1306.hpp"
#include "../graphic/canvas8.hpp"
#include "stm32f1_mcu.hpp"
#include "../di.hpp"
namespace di = boost::di;

const auto nm_display = [] {};
const auto nm_dbus_uart = [] {};

using namespace di;

// injections
auto breakout_injector = make_injector(
    //display ports need by ssd1306
    bind<digitalport>.named(dp_datacmd).to<stm32f1_port_b7>(),
    bind<digitalport>.named(dp_reset).to<stm32f1_port_b6>(),
    bind<digitalport>.named(dp_select).to<stm32f1_port_b8>(),

    bind<mcu>.to<stm32f1>(),
    bind<buffer8>.to<ssd1306_framebuffer>(),
    bind<display>.named(nm_display).to<ssd1306>(),
    bind<databus>.named(nm_dbus_uart).to<stm32f1_uart2>(),
    bind<databus>.named(nm_dbus_display).to<stm32f1_spi1>()
);
