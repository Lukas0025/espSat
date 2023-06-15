/**
 * espSat project simple esp base satellite
 * File with implementation of sensor manager
 * @author Lukas Plevac <lukas@plevac.eu>
 */

#include "instruments.h"
#include "persistMem.h"
#include "testData.h"
#include <Wire.h>
#include <ErriezBMX280.h>

namespace instruments {

    ErriezBMX280 bmpInstrument = ErriezBMX280(BMX280_ADDR);

    void scanI2CBus() {    
      DEBUG_PRINT("Performing I2C BUS Scan");

      for(byte address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        byte error = Wire.endTransmission();
    
        if (error == 0) DEBUG_PRINT("I2C device found at address ",  address);
        if (error == 4) DEBUG_PRINT("I2C Unknown error at address ", address);
      }

      DEBUG_PRINT("I2C BUS Scan done");
    }

    void setup() {
      PersistMem::init();

      // Init I2C Bus
      Wire.begin(INNER_I2C_SDA, INNER_I2C_SCL);
      Wire.setClock(INNER_I2C_CLOCK);

      #ifdef DEBUG
        scanI2CBus();
      #endif

      // reconfigure pin for analog input
      analogRead(INNER_ADC);

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
      float alt = bmpInstrument.readAltitude(SEA_LEVEL_PRESSURE_HPA);
      DEBUG_PRINT("Readed alt is: ", alt, "m");
      return alt;
    }

    float getPressure() {
      float press = bmpInstrument.readPressure() / 100.0F;
      DEBUG_PRINT("Readed pressure is: ", press, "hPa");
      return bmpInstrument.readPressure();
    }

    float getVoltage() {
      float sysVoltage = (analogRead(INNER_ADC) * ADC_V_MAX * ADC_R_RATIO) / ADC_BITS;
      DEBUG_PRINT("Readed system voltage is: ", sysVoltage, "V");
      return sysVoltage;
    }

    float getTemperature() {  
      float temp = bmpInstrument.readTemperature();
      DEBUG_PRINT("Readed temperature is: ", temp, "C");
      return temp;
    }

    /*
     * Functions for work with satellite counters
     */

    uint32_t getTransmitCounter() {
      return PersistMem::getTransmitCounter();
    }

    uint32_t getLoraCounter() {
      return PersistMem::getLoraCounter();
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
      PersistMem::setBootCounter(++bc);

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
