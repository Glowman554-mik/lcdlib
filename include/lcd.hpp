#pragma once

#include <cstdint>
#include <software_i2c.hpp>
#include <mbed.h>

#define LCD_CURSOR 0b00000010
#define LCD_CURSOR_BLINK 0b00000001

class software_lcd {
    public:
        software_lcd(PinName sda, PinName scl);
        software_lcd();

        void set_flags(uint8_t flags);

        void clear();
        void cursor(int pos);

        void putc(char c);
        void puts(const char* c);

    private:
        uint8_t address;
        software_i2c i2c;

        void wait();

        void send_byte(char b, uint8_t rw, uint8_t rs);
        void send_nippel(char b, uint8_t rw, uint8_t rs);

        void init(); 
};
