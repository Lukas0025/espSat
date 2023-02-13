/**
 * espSat project simple esp base satellite
 * File with header of sensor manager
 * @author Lukas Plevac <lukas@plevac.eu>
 */
#pragma once

#include <Arduino.h>
#include "./telemetry.h"
#include "./config.h"

namespace instruments {

    /**
     * Setup all instruments
     */
    void setup();

    /**
     * get value of transmission counter
     * @return value of counter
     */
    uint32_t getTransmitCounter();

    /**
     * get value of lora transmission counter
     * @return value of counter
     */
    uint32_t getLoraCounter();

    /**
     * get value of boot counter
     * @return value of counter
     */
    uint16_t getBootCounter();
    
    /**
     * get value of transmission counter and increment it
     * @post transmition counter += 1
     * @return value of counter
     */
    uint32_t incGetTransmitCounter();

    /**
     * get value of lora transmission counter and increment it
     * @post lora transmition counter += 1
     * @return value of counter
     */
    uint32_t incGetLoraCounter();
    
    /**
     * get value of boot counter and increment it
     * @post boot counter += 1
     * @return value of counter
     */
    uint16_t incGetBootCounter();

    /**
     * get uptime of sattelite in ms
     * @return satellite uptime in ms
     */
    unsigned getUptime();

    /**
     * get current altudite of satellite in meters
     * @return altudite in m
     */
    float getAlt();

    /**
     * get current pressure in hpa
     * @return pressure in hpa
     */
    float getPressure();

    /**
     * get current voltage of satellite system in V
     * @return voltage in V
     */
    float getVoltage();

    /**
     * get current temperature of enviroment in C
     * @return temperature in C 
     */
    float getTemperature();
    

    /**
     * get UPTIME of satellite in seconds for telemetry
     * @return String
     */
    String uptimeStr();
    /**
     * get voltage of sattelite systems for telemetry
     * @return string
     */
    String voltageStr();
    
    /**
     * get teperature of enviroment for telemetry
     * @return String
    */
    String temperatureStr();
    
    /**
     * get satellite altudite for telemetry
     * @return string
     */
    String altitudeStr();
    
    /**
     * get enviroment presure for telemetry
     * @return string
     */
    String pressureStr();

    /**
     * get lora trinsmition counter for telemetry
     * @return string
     */
    String loraCounterStr();
    
    /**
     * get transmition counter for telemetry
     * @return string
     */
    String transmitCounterStr();

    /**
     * get boot counter for telemetry
     * @return string
     */
    String bootCounterStr();

    /**
     * Add standart instruments to telemetry
     * @post in telemetry is added telemetry instruments
     */
    void autoAddToTelemetry(Telemetry *telemetry);
}
