/**
 * espSat project simple esp base satellite
 * File with implementation of sensor manager
 * @author Lukas Plevac <lukas@plevac.eu>
 */

#include "instruments.h"
#include "persistMem.h"
#include "testData.h"
#include <Wire.h>
#include <BMP180.h>

namespace instruments {

    const float sea_press = 1013.25;
    BMP180 bmpInstrument(BMP180_ULTRAHIGHRES);

    void setup() {
      PersistMem::init();

      Wire.begin(INNER_I2C_SDA, INNER_I2C_SCL);

      for (unsigned retry = 0; retry < 3; retry++) {
        if (bmpInstrument.begin()) {
          DEBUG_PRINT("BMP inited");
          break;
        }

        ERROR_PRINT("fail to init BMP sensor");
        delay(5000);
      }
    }

    /*
     * Functions for work with satellite sensors
     */

    unsigned getUptime() {
      return millis();
    }

    unsigned getExpectedRunTime() {
      return PersistMem::getTransmitCounter() * 30;
    }

    float getAlt() {
      return ((pow((sea_press / bmpInstrument.getPressure()), 1/5.257) - 1.0) * (bmpInstrument.getTemperature() + 273.15)) / 0.0065;
    }

    float getPressure() {
      return bmpInstrument.getPressure();
    }

    float getVoltage() {
      return 3;
    }

    float getTemperature() {
      return bmpInstrument.getTemperature();
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
      return intAs2Str(getExpectedRunTime() / 3600)    + ":" + // Hour
             intAs2Str(getExpectedRunTime() / 60 % 60) + ":" + // Min
             intAs2Str(getExpectedRunTime() % 60);             // Sec
    }

    String latStr()              { return "0.0"; }
    String lonStr()              { return "0.0"; }
    String voltageStr()          { return String((int)(getVoltage()     * 100)); }
    String temperatureStr()      { return String((int)(getTemperature() * 10 )); }
    String altitudeStr()         { return String((int)getAlt()               );  }
    String pressureStr()         { return String((int)getPressure()          );  }
    String loraCounterStr()      { return String(getLoraCounter()            );  }
    String transmitCounterStr()  { return String(getTransmitCounter()        );  }
    String bootCounterStr()      { return String(getBootCounter()            );  }

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
