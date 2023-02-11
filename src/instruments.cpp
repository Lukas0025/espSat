#include "instruments.h"

namespace instruments {
    unsigned tc;
    unsigned lc;
    
    /**
     * Setup all instruments
     */
    void setup() {
      tc = 0;
      lc = 0;
    }

    unsigned getAlt() { return 1000; }

    unsigned getTransmitCounter() { return tc; }

    void incTransmitCounter() {tc++;}

    unsigned incGetLoraCounter() {return lc++;}

    

    /**
     * Return UPTIME of satellite in seconds
     * @return String
     */
    String uptime() { return String(millis() / 1000); }

    /**
     * Return voltage of sattelite systems
     * @return string
     */
    String voltage() { return String("3.3V"); }

    /**
     * Return outer teperature
     * @return String
    */
    String temperature() { return "10.01C"; }
    
    /**
     * Return altudite
     */
    String altitude() { return String(getAlt()) + "M"; }
    
    /**
     * Return presure
     */
    String pressure()  { return "1000hpa"; }

    String loraCounterStr()     { return String(lc); }
    String transmitConterStr()  { return String(tc); }

    /**
     * Add standart instruments to telemetry
     */
    void autoAddToTelemetry(Telemetry *telemetry) {
      telemetry->addInstrument("uptime", uptime);
      telemetry->addInstrument("voltage", voltage);
      telemetry->addInstrument("temperature", temperature);
      telemetry->addInstrument("altitude", altitude);
      telemetry->addInstrument("pressure", pressure);
      telemetry->addInstrument("lora-counter", loraCounterStr);
      telemetry->addInstrument("transmit-counter", transmitConterStr);
    }
}
