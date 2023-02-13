/**
 * espSat project simple esp base satellite
 * File with implementation of persistent memory manager
 * @author Lukas Plevac <lukas@plevac.eu>
 */

#include "persistMem.h"
#include <EEPROM.h>
#include "debug.h"

namespace PersistMem {

    bool inited = false;

    void init() {
        EEPROM.begin(EEPROM_SIZE);
        inited = true;
    }

    void reset() {
        uint8_t zero[EEPROM_SIZE] = {0};
        write(zero, 0, EEPROM_SIZE);
    }

    void write(uint8_t *data, unsigned addr, unsigned size) {
        
        if (!inited) {
            WARNING_PRINT("Writing ", size, "B to not inited memory on addr ", addr);
        }

        for (unsigned i = 0; i < size; i++) {
            if (EEPROM.read(addr + i) != data[i]) EEPROM.write(addr + i, data[i]);
        }

        DEBUG_PRINT("Writed ", size, "B on addr ", addr);
        EEPROM.commit();
    }

    void read(uint8_t *buffer, unsigned addr, unsigned size) {
        if (!inited) {
            WARNING_PRINT("Reading ", size, "B from not inited memory on addr ", addr);
        }

        for (unsigned i = 0; i < size; i++) {
            buffer[i] = EEPROM.read(addr + i);
        }

        DEBUG_PRINT("Readed ", size, "B from addr ", addr);
    }

    uint16_t getBootCounter() {
        //crate buffer
        uint16_t value;
        //read from memory to buffer
        read((uint8_t*)&value, Addr::bootCounter, WORD_SIZE);

        return value;
    }

    void setBootCounter(uint16_t value) {
        write((uint8_t*)&value, Addr::bootCounter, WORD_SIZE);
    }

    StateDrive::state_t (*getLastState())() {
        //crate buffer
        uint16_t stateId;
        //read from memory to buffer
        read((uint8_t*)&stateId, Addr::lastState, WORD_SIZE);

        if (stateId == 0) return StateDrive::initState;
        if (stateId == 1) return StateDrive::idleState;

        return StateDrive::initState;
    }

    void setLastState(StateDrive::state_t (*state)()) {
        //crate buffer
        uint16_t stateId;

        if      (state == StateDrive::initState)          stateId = 0;
        else if (state == StateDrive::idleState)          stateId = 1;
        else                                              stateId = 0;

        write((uint8_t*)&stateId, Addr::lastState, WORD_SIZE);
    }
    
    uint32_t getTransmitCounter() {
        //crate buffer
        uint32_t value;
        //read from memory to buffer
        read( (uint8_t*)&value, Addr::transmitCounter, DOUBLE_WORD_SIZE);

        return value;
    }

    void setTransmitCounter(uint32_t value) {
        write((uint8_t*)&value, Addr::transmitCounter, DOUBLE_WORD_SIZE);
    }

    uint32_t getLoraCounter() {
        //crate buffer
        uint32_t value;
        //read from memory to buffer
        read((uint8_t*)&value, Addr::loraCounter, DOUBLE_WORD_SIZE);

        return value;
    }

    void setLoraCounter(uint32_t value) {
        write((uint8_t*)&value, Addr::loraCounter, DOUBLE_WORD_SIZE);
    }

}