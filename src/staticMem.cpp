#include "staticMem.h"
#include <EEPROM.h>

namespace StaticMem {

    void write(uint8_t *data, unsigned addr, unsigned size) {
        for (unsigned i = 0; i < size; i++) {
            EEPROM.write(addr + i, data[i]);
        }
    }

    void read(unsigned addr, unsigned size, uint8_t *buffer) {
        for (unsigned i = 0; i < size; i++) {
            buffer[i] = EEPROM.read(addr + i);
        }
    }

}