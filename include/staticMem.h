#pragma once
#include <Arduino.h>

namespace StaticMem {
    namespace Addr {
        /**
         * EEPROM MAP
         *    0       1        2       3       5        6        7     8
         * | BOOT COUNTER | Current State | Transmit Counter | Lora Counter |
         */
        const unsigned bootCounter     = 0x0;
        const unsigned currentState    = 0x2;
        const unsigned transmitCounter = 0x4;
    }

    void write(uint8_t *data, unsigned addr, unsigned size);
    void read(unsigned addr, unsigned size, uint8_t *buffer);

} 