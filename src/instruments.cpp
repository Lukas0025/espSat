/**
 * espSat project simple esp base satellite
 * File with implementation of sensor manager
 * @author Lukas Plevac <lukas@plevac.eu>
 */

#include "instruments.h"
#include "persistMem.h"

namespace instruments {

    void setup() {
      PersistMem::init();
    }

    /*
     * Functions for work with satellite sensors
     */

    unsigned getUptime() {
      return millis();
    }

    float getAlt() {
      return 1000;
    }

    float getPressure() {
      return 1100;
    }

    float getVoltage() {
      return 3.3;
    }

    float getTemperature() {
      return 10.01;
    }

    /*
     * Functions for work with satellite counters
     */

    uint32_t getTransmitCounter() {
      return PersistMem::getTransmitCounter();
    }

    uint32_t getLoraCounter() {
      return PersistMem::getTransmitCounter();
    }

    uint16_t getBootCounter() {
      return PersistMem::getBootCounter();
    }

    uint32_t incGetTransmitCounter() {
      auto tc = PersistMem::getTransmitCounter(); 
      PersistMem::setTransmitCounter(++tc);

      return tc;
    }

    uint32_t incGetLoraCounter() {
      auto lc = PersistMem::getLoraCounter(); 
      PersistMem::setLoraCounter(++lc);

      return lc;
    }

    uint16_t incGetBootCounter() {
      auto bc = PersistMem::getBootCounter(); 
      PersistMem::setLoraCounter(++bc);

      return bc;
    }

    /*
     * This is telemetry support functions return state of senzor in String
     */

    String intAs2Str(unsigned val) {
      if (val > 99) val = val % 100;
      if (val < 10) return "0" + String(val);
      
      return String(val);
    }

    String uptimeStr()           { 
      return intAs2Str(getUptime() / 1000 / 3600)    + ":" + // Hour
             intAs2Str(getUptime() / 1000 / 60 % 60) + ":" + // Min
             intAs2Str(getUptime() / 1000 % 60);             // Sec
    }

    String latStr()              { return "0.0"; }
    STring lonStr()              { return "0.0"; }
    String voltageStr()          { return String(getVoltage()); }
    String temperatureStr()      { return String(getTemperature()); }
    String altitudeStr()         { return String(getAlt()); }
    String pressureStr()         { return String(getPressure()); }
    String loraCounterStr()      { return String(getLoraCounter()); }
    String transmitCounterStr()  { return String(getTransmitCounter()); }
    String bootCounterStr()      { return String(getBootCounter()); }

    /*
     * Add standart instruments to telemetry
     */
    void autoAddToTelemetry(Telemetry *telemetry) {
      telemetry->addInstrument("TCOUNTER",    transmitCounterStr);
      telemetry->addInstrument("TIME",        uptimeStr);
      telemetry->addInstrument("LAT",         latStr);
      telemetry->addInstrument("LON",         lonStr);
      telemetry->addInstrument("ALT",         altitudeStr);
      telemetry->addInstrument("voltage",     voltageStr);
      telemetry->addInstrument("temperature", temperatureStr);
      telemetry->addInstrument("pressure",    pressureStr);
      telemetry->addInstrument("lcounter",    loraCounterStr);
      telemetry->addInstrument("bcounter",    bootCounterStr);
    }
}
