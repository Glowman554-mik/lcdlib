#pragma once

#include "mbed.h"
#include <stdint.h>

class software_i2c {
    public:
        software_i2c(PinName sda, PinName scl);
 
        void read(uint8_t device_address, uint8_t* data, uint8_t data_bytes);
        void write(uint8_t device_address, uint8_t* data,  uint8_t data_bytes);
        
        void write_u8(uint8_t device_address, uint8_t byte);

        void set_device_address(uint8_t address);
        void set_frequency(uint32_t frequency);

        inline void init() {
            scl.output();
            sda.output();

            sda = true;
            scl = false;
            wait_for(frequency_delay);

            for (int i = 0; i < 4; i++) {
                stop();
            }
        }

    private:
        inline void wait_for(int ammount) {
            for (int i = 0; i < ammount * 5; i++) {
                __asm__ __volatile__("nop");
            }
        }

        inline void start() {
            sda.output();
            wait_for(frequency_delay);

            scl = true;
            sda = true;
            wait_for(frequency_delay);

            sda = false;
            wait_for(frequency_delay);

            scl = false;
            wait_for(frequency_delay);
        }

        inline void stop() {
            sda.output();
            wait_for(frequency_delay);

            sda = false;
            wait_for(frequency_delay);

            scl = true;
            wait_for(frequency_delay);

            sda = true;
        }

        inline void put_byte(uint8_t byte) {
            sda.output();
            for (int i = 8; i > 0; --i) {
                wait_for(frequency_delay);
                
                sda = byte & (1 << (i - 1));
                wait_for(frequency_delay);

                scl = true;
                wait_for(frequency_delay);

                scl = false;
            }

            sda = true;
        }

        inline uint8_t read_byte() {
            uint8_t byte = 0;

            sda.input();
            sda.mode(OpenDrain);
            wait_for(frequency_delay);

            for (int i = 8; i > 0; --i) {
                scl = true;
                wait_for(frequency_delay);

                byte |= sda << (i - 1);
                wait_for(frequency_delay);

                scl = false;
                wait_for(frequency_delay);
            }

            sda.output();

            return byte;
        }

        inline void send_ack() {
            sda.output();
            wait_for(frequency_delay);

            sda = false;
            wait_for(frequency_delay);

            scl = true;
            wait_for(frequency_delay);

            scl = false;
            sda = true;
        }

        inline bool recv_ack() {
            sda.output();
            sda = true;
            scl = true;

            sda.input();
            sda.mode(OpenDrain);
            wait_for(frequency_delay);

            scl = false;

            if (sda) {
                return false;
            } else {
                wait_for(frequency_delay);
                return true;
            }
        }

        DigitalInOut sda;
        DigitalInOut scl;
    
        uint8_t device_address;
        uint32_t frequency_delay;
};
