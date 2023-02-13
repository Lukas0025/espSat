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
    String uptimeStr()           { return String(getUptime() / 1000) + "S"; }
    String voltageStr()          { return String(getVoltage()) + "V"; }
    String temperatureStr()      { return String(getTemperature()) + "C"; }
    String altitudeStr()         { return String(getAlt()) + "M"; }
    String pressureStr()         { return String(getPressure()) + "hpa"; }
    String loraCounterStr()      { return String(getLoraCounter()); }
    String transmitCounterStr()  { return String(getTransmitCounter()); }
    String bootCounterStr()      { return String(getBootCounter()); }

    /*
     * Add standart instruments to telemetry
     */
    void autoAddToTelemetry(Telemetry *telemetry) {
      telemetry->addInstrument("uptime",      uptimeStr);
      telemetry->addInstrument("voltage",     voltageStr);
      telemetry->addInstrument("temperature", temperatureStr);
      telemetry->addInstrument("altitude",    altitudeStr);
      telemetry->addInstrument("pressure",    pressureStr);
      telemetry->addInstrument("lcounter",    loraCounterStr);
      telemetry->addInstrument("tcounter",    transmitCounterStr);
      telemetry->addInstrument("bcounter",    bootCounterStr);
    }
}
