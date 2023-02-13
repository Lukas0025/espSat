/**
 * espSat project simple esp base satellite
 * File with header of persistent memory manager
 * @author Lukas Plevac <lukas@plevac.eu>
 */
#pragma once
#include <Arduino.h>
#include "stateDrive.h"

#define WORD_SIZE 2
#define DOUBLE_WORD_SIZE 4

#define EEPROM_SIZE 15

namespace PersistMem {
    namespace Addr {
        /**
         * EEPROM MAP
         *      | 0x0  |  0x1  |  0x2  |  0x3  |  0x4  |  0x5  |  0x6  |  0x7  |  0x8  |  0x9  |  0xA  |  0xB  |  0xC  |  0xD  |  0xE  |  0xF        
         * 0x0  | BOOT COUNTER |  LAST STATE   |       TRANS COUNTER           |        LORA COUNTER           |
         */
        const unsigned bootCounter     = 0x00;
        const unsigned lastState       = 0x02;
        const unsigned transmitCounter = 0x04;
        const unsigned loraCounter     = 0x08;
    }

    /**
     * Init persistent memory
     * @post persistent memory is inited
     */
    void init();

    /**
     * Write 0 to persistent memory on all addreses
     * @post persistent memory is cleared
     */
    void reset();

    /**
     * Write data object to persistent memory
     * @param data pointer to data object
     * @param addr addres to write in persistent memory
     * @param size size of data object
     */
    void write(uint8_t *data, unsigned addr, unsigned size);

    /**
     * Read data object from persistent memory
     * @param buffer pointer of empty data object for load from memory
     * @param addr addres to read in persistent memory
     * @param size size of data object
     */
    void read(uint8_t *buffer, unsigned addr, unsigned size);

    /**
     * Get boot counter from persistent memory
     * @return boot counter
     */
    uint16_t getBootCounter();
    
    /**
     * set boot counter to persistent memory
     * @param value value to write
     */
    void     setBootCounter(uint16_t value);

    /**
     * Get last state of state machine from persistent memory
     * @return pointer to state
     */
    void (*getLastState())();

    /**
     * set last state of state machine to persistent memory
     * @param state pointer to state
     */
    void setLastState(void (*state)());
    
    /**
     * Get transmition counter from persistent memory
     * @return transmition counter
     */
    uint32_t getTransmitCounter();
    
    /**
     * set transmition counter to persistent memory
     * @param value value to write
     */
    void setTransmitCounter(uint32_t value);

    /**
     * Get lora transmition counter from persistent memory
     * @return lora transmition
     */
    uint32_t getLoraCounter();
    
    /**
     * set lora transmition counter to persistent memory
     * @param value value to write
     */
    void setLoraCounter(uint32_t value);
} 