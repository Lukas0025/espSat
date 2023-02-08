#pragma once

#include <Arduino.h>
#include "./telemetry.h"
#include "./config.h"

namespace instruments {

    /**
     * Setup all instruments
     */
    void setup();

    unsigned getAlt();

    unsigned getTransmitCounter();

    void incTransmitCounter();

    unsigned incGetLoraCounter();
    

    /**
     * Return UPTIME of satellite in seconds
     * @return String
     */
    String uptime();
    /**
     * Return voltage of sattelite systems
     * @return string
     */
    String voltage();
    /**
     * Return outer teperature
     * @return String
    */
    String temperature();
    
    /**
     * Return altudite
     */
    String altitude();
    
    /**
     * Return presure
     */
    String pressure();

    String loraCounterStr() ;
    String transmitConterStr() ;

    /**
     * Add standart instruments to telemetry
     */
    void autoAddToTelemetry(Telemetry *telemetry);
}
