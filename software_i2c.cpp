#include <software_i2c.hpp>

software_i2c::software_i2c(PinName _sda, PinName _scl) : sda(_sda), scl(_scl) {
    scl.output();
    scl.mode(OpenDrain);
    sda.output();
    sda.mode(OpenDrain);

    device_address = 0;

    frequency_delay = 60;

    init();
}

void software_i2c::read(uint8_t _device_address, uint8_t *data, uint8_t data_bytes) {
    if (data == 0 || data_bytes == 0) {
        return;
    }

    _device_address |= 0x01;
    
    __disable_irq();
    start();

    put_byte(_device_address);
    recv_ack();

    for (int x = 0; x < data_bytes; x++) {
        data[x] = read_byte();
        if (x < (data_bytes - 1)) {
            send_ack();
        }
    }

    stop();
    __enable_irq();
}

void software_i2c::write(uint8_t _device_address, uint8_t *data, uint8_t data_bytes) {
    if (data == 0 || data_bytes == 0) {
        return;
    }

    _device_address &= 0xfe;

    __disable_irq();
    start();

    put_byte(_device_address);
    recv_ack();
    
    for (int x = 0; x < data_bytes; x++) {
        put_byte(data[x]);
        recv_ack();
    }

    stop();
    __enable_irq();
}

void software_i2c::write_u8(uint8_t _device_address, uint8_t byte) {
    _device_address &= 0xfe;

    __disable_irq();
    start();

    put_byte(_device_address);
    recv_ack();

    put_byte(byte);
    recv_ack();

    stop();
    __enable_irq();
}

void software_i2c::set_frequency(uint32_t frequency) {
    this->frequency_delay = 1000000 / frequency;
}

void software_i2c::set_device_address(uint8_t address) {
    this->device_address = address;
}
