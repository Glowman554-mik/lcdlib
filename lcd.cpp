#include <lcd.hpp>

software_lcd::software_lcd(PinName sda, PinName scl) : i2c(sda, scl) {
    init();
}

software_lcd::software_lcd() : i2c(PA_12, PA_11) {
    init();
}

void software_lcd::clear() {
    send_byte(0x01, 0, 0);
    cursor(0);
}

void software_lcd::putc(char c) {
    send_byte(c, 0, 1);
}

void software_lcd::puts(const char* c) {
    while (*c != 0) {
        putc(*c++);
    }
}

void software_lcd::wait() {
    __asm__ __volatile__ (
        "push {R0, R1};"
        "ldr R0, =32000000 / 5 / 1000;"
        "mov R1, #1;"
        "1:;"  
        "subs R0, R1;"
        "bne 1b;"
        "pop {R0, R1};"
    );  
}

void software_lcd::cursor(int pos) {
    send_byte(0x80 + pos, 0, 0);
}

void software_lcd::init() {

    uint8_t data[1];
    for (address = 0; address < 255 && data[0] != 0x55; address++) {
        i2c.write_u8(address, 0x55);
        i2c.read(address, data, 1);
    }

    wait_us(20000);
    send_nippel(0b0011, 0, 0);
    wait_us(5000);
    send_nippel(0b0011, 0, 0);
    wait_us(1000);
    send_nippel(0b0011, 0, 0);
    wait_us(150);
    send_nippel(0b0010, 0, 0);
 
    send_byte(0b00101000, 0, 0); // 4 bit 2 lines
    send_byte(0b00000001, 0, 0); // display clear
    send_byte(0b00000110, 0, 0); // increment cursor
    send_byte(0b10000000, 0, 0); // home    
    send_byte(0b00001100, 0, 0); // Display On

    cursor(0x0);
}

void software_lcd::set_flags(uint8_t flags) {
    send_byte(0b00001100 | flags, 0, 0); // Display On
}

void software_lcd::send_byte(char b, uint8_t rw, uint8_t rs) {
    uint8_t val;

    val = (b & 0xF0) + 0x08 + ((rw & 0x01) << 1) + (rs & 0x01);
    i2c.write_u8 (address, val);
    wait();

    val = (b & 0xF0) + 0xC + ((rw & 0x01) << 1) + (rs & 0x01);
    i2c.write_u8(address, val);
    wait();
    
    val = (b & 0xF0) + 0x8 + ((rw & 0x01) << 1) + (rs & 0x01);
    i2c.write_u8(address, val);
    wait();
    
    val = ((b & 0xF) << 4) + 0x8 + ((rw & 0x01) << 1) + (rs & 0x01);
    i2c.write_u8(address, val);
    wait();
    
    val = ((b & 0xF) << 4) + 0xC + ((rw & 0x01) << 1) + (rs & 0x01);
    i2c.write_u8(address, val);
    wait();
    
    val = ((b & 0xF) << 4) + 0x8 + ((rw & 0x01) << 1) + (rs & 0x01);
    i2c.write_u8(address, val);
    wait();
}

void software_lcd::send_nippel(char b, uint8_t rw, uint8_t rs) {
    uint8_t val;

    val = ((b & 0xF) << 4) + 0x0 + ((rw & 0x01) << 1) + (rs & 0x01);
    i2c.write_u8(address, val);
    wait();
    
    val = ((b & 0xF) << 4) + 0x4 + ((rw & 0x01) << 1) + (rs & 0x01);
    i2c.write_u8(address, val);
    wait();

    val = ((b & 0xF) << 4) + 0x0 + ((rw & 0x01) << 1) + (rs & 0x01);
    i2c.write_u8(address, val);
    wait();
}
