
#pragma once

#include "../intfs/mcu.hpp"
#include "../intfs/ports.hpp"
#include "../intfs/databus.hpp"
#include "../intfs/buffer8.hpp"
#include "../intfs/display.hpp"
#include "../graphic/envelope.hpp"

class ssd1306_framebuffer {
protected:
    envelope_u16 changes;
    uint8_t buff[8][128];

public:
    ssd1306_framebuffer() {
        clear();
    }

    uint16_t height() { return 8; }
    uint16_t width() { return 128; }
    
    void clear() {
        uint8_t row = 0u;
        while (row < height()) {
            uint8_t col = 0u;
            while (col < width()) { 
                buff[row][col] = 0;
                col++;
            }
            row++;
        }
        changes.expand(0,0);
        changes.expand(width()-1, height()-1);
    }

    void reset_changes() {
        changes.clear();
    }

    envelope_u16 get_changes() {
        return changes;
    }

    uint8_t get(uint16_t row, uint16_t col) {
        return buff[row][col];
    }
    
    void set(uint16_t row, uint16_t col, uint8_t v) {
        buff[row][col] = v;
        changes.expand(col, row);
    }
};

template<databus db, digitalport dp_datacmd, digitalport dp_reset, digitalport dp_select, mcu mcu, buffer8 buffer8>
class ssd1306 {
public:
    mcu& mmcu;
    buffer8& framebuffer;

    // ports used by the SPI display
    dp_datacmd& datacmd;
    dp_reset& reset;
    dp_select& select;

    // SPI or I2C bus
    db& dbus;

    // display address for I2C
    uint8_t address = 0;

public:

    ssd1306(mcu& mmcu, dp_datacmd& datacmd, dp_reset& reset, dp_select& select,
        db& dbus, buffer8& framebuffer) : mmcu(mmcu), datacmd(datacmd), reset(reset), select(select), dbus(dbus), framebuffer(framebuffer) {
    }

    enum class commands : uint8_t {
        // powerstate
        SLEEP = 0xAE,
        ON = 0xAF,
        
        // i2c_control
        CMD_STREAM = 0x0,
        DATA_STREAM = 0x40,

        // commands
        SET_DISP_START_LINE	    = 0x40,
        SET_SEG_REMAP_0  	    = 0xA0,
        SET_SEG_REMAP_127	    = 0xA1,
        SET_MULTIPLEX_RATIO     = 0xA8,
        SET_COM_SCAN_NORMAL	    = 0xC0,
        SET_COM_SCAN_INVERTED	= 0xC8,
        SET_VERTICAL_OFFSET     = 0xD3,
        SET_WIRING_SCHEME       = 0xDA,
        SET_DISP_CLOCK          = 0xD5,
        SET_PRECHARGE_PERIOD    = 0xD9,
        SET_VCOM_DESELECT_LEVEL = 0xDB,
        SET_CONTRAST		    = 0x81,
        RESUME_TO_RAM_CONTENT	= 0xA4,
        DISPLAY_MODE_NORMAL     = 0xA6,
        DISPLAY_MODE_INVERTED   = 0xA7,
        MEM_ADDRESSING 		    = 0x20,
        SET_COLUMN_ADDRESS      = 0x21,
        CHARGE_PUMP_REGULATOR 	= 0x8D,
        CHARGE_PUMP_ON   	    = 0x14,
        SET_PAGE_ADDR           = 0x22,
        SET_COL_LO_NIBBLE       = 0x00,
        SET_COL_HI_NIBBLE       = 0x10,
        DEACTIVATE_SCROLL       = 0x2E,
        NOP                     = 0xE3
    };

    uint16_t rows() {
        return framebuffer.height()*8;
    }

    uint16_t columns() {
        return framebuffer.width();
    }

    void set_address(uint8_t a) {
        address = a;
    }

    void init_display() {
        datacmd.mode(port_mode::output);
        reset.mode(port_mode::output);
        select.mode(port_mode::output);

        reset_display();

        commands init_commands[] = {
            commands::SLEEP,
            commands::SET_DISP_CLOCK, (commands)0x80,
            commands::SET_MULTIPLEX_RATIO, (commands)0x3F,
            commands::SET_VERTICAL_OFFSET, (commands)0x00,
            commands::SET_DISP_START_LINE,
            commands::CHARGE_PUMP_REGULATOR,
            commands::CHARGE_PUMP_ON,
            commands::MEM_ADDRESSING, (commands)0x00, // Horizontal memory addressing mode
            commands::SET_SEG_REMAP_0,
            commands::SET_COM_SCAN_INVERTED,
            commands::SET_WIRING_SCHEME, (commands)0x12,
            commands::SET_CONTRAST, (commands)0x8F,
            commands::SET_PRECHARGE_PERIOD, (commands)0x22,
            commands::SET_VCOM_DESELECT_LEVEL, (commands)0x40,
            commands::RESUME_TO_RAM_CONTENT,
            commands::DISPLAY_MODE_NORMAL,
            commands::DEACTIVATE_SCROLL,
            commands::ON
        };

        write_commands(init_commands, sizeof(init_commands)/sizeof(commands));
    }

    void set_orientation(displayorientation o) {
        commands cmds[2];
        
        if (o == displayorientation::NORMAL or
            o == displayorientation::NORMAL_MIRRORED) {
            cmds[0] = commands::SET_SEG_REMAP_0;
        } else {
            cmds[0] = commands::SET_SEG_REMAP_127;
        }

        if (o == displayorientation::NORMAL_MIRRORED or
            o == displayorientation::UPSIDE_DOWN) {
            cmds[1] = commands::SET_COM_SCAN_INVERTED;
        } else {
            cmds[1] = commands::SET_COM_SCAN_NORMAL;
        }

        write_commands(cmds, 2);
    }

    void set_display_mode(bool inverted) {
        commands cmds;
        if (inverted) {
            cmds = commands::DISPLAY_MODE_INVERTED;
        } else {
            cmds = commands::DISPLAY_MODE_NORMAL;
        }
        write_commands(&cmds, 1);
    }

    void set_contrast(uint8_t c) {
        commands cmds[] = {commands::SET_CONTRAST, commands(c)};
        write_commands(cmds, 2);
    }

    void reset_display() {
        if (dbus.get_protocol() == databus_protocol::SPI) {
            reset.set(true);
            mmcu.wait_ms(10);
            reset.set(false);
            mmcu.wait_ms(10);
            reset.set(true);
        }
    }

    void write_commands(commands cmds[], uint8_t cmds_size) {
        datacmd.set(false);
        select.set(false);

        dbus.start_transaction(address);
        if (dbus.get_protocol() == databus_protocol::I2C) {
            //dbus.write(i2c_control::CMD_STREAM);
        }

        dbus.write_array((char*)cmds, cmds_size);

        dbus.end_transaction();
        select.set(true);
    }

    void reset_cursor(uint16_t lx, uint16_t ly, uint16_t ux, uint16_t uy) {
        commands cmds[] = {
            // set page update range
            commands::SET_PAGE_ADDR, (commands)ly, (commands)uy,
            // set column update range
            commands::SET_COLUMN_ADDRESS, (commands)lx, (commands)ux
        };
        write_commands(cmds, 6);
    }

    void clear() {
        framebuffer.clear();
        update_frame();
    }

    void update_frame() {
        auto ev = framebuffer.get_changes();
        if (ev.empty()) {
            return;
        }
        reset_cursor(ev.lx, ev.ly, ev.ux, ev.uy);

        datacmd.set(true);
        select.set(false);
        dbus.start_transaction(address);
        if (dbus.get_protocol() == databus_protocol::I2C) {
            //dbus.write(i2c_control.DATA_STREAM);
        }

        auto row = ev.ly;
		while (row <= ev.uy) {
			auto col = ev.lx;
			while (col <= ev.ux) { 
                auto v = framebuffer.get(row, col);
				dbus.write(v);
				col++;
			}
			row++;
		}
        
        dbus.end_transaction();
        select.set(true);
        framebuffer.reset_changes();
    }
};
