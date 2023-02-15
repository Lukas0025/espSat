/**
 * espSat project simple esp base satellite
 * File with header of telemetry manager
 * @author Lukas Plevac <lukas@plevac.eu>
 */

#pragma once
#include <Arduino.h>
#define MAXINSTRUMENTS 10

class Telemetry {
    public:
        /**
         * Telemetry object constructor
         * @param craftName String of name if craft
         * @param beginString String what is trasmited before data
         * @param endString String what is transmited after data
         */
        Telemetry(String craftName, String beginString, String endString) {
          craftName.toUpperCase();
          beginString.toUpperCase();
          endString.toUpperCase(); 
          
          this->craftName   = craftName;
          this->beginString = beginString;
          this->endString   = endString; 
          this->payloadLen  = 0;
        }

        /**
         * Add instrument to telemetry to track instrument using telemetry
         * @param name String of name of instrument
         * @param payloadGet Pointer to function what return state of instrument in String
         */
        void addInstrument(String name, String (*payloadGet)());

        /**
         * GetCurrent state of telemetry in string to transmit
         * @return String of state
         */
        String getState();

        /**
         * GetCurrent state of telemetry in UKHAS format string to transmit
         * Format is $$<payload>,<data>,<data>,...,<last data>*<checksum>\n
         * where $$ is beginString and \n is end string
         * @return String of state in UKHAS format
         */
        String getUKHAS();

        /**
         * Check if instriments is in correct order for UKHAS telemetry format
         * Recomended format is $$<payload>,<message number>,<time>,<latitude>,<longitude>,<altitude>,<data>,...,<last data>*<checksum>
         * @return bool True if is ok 
         */
        bool verifyUKHAS();

        /**
         * Get CRC of UKHAS message
         * @param message string for CRC calculation
         * @return uint16_t CRC 
         */
        uint16_t getUKHASCRC(String message);

    private:
        String craftName;
        String beginString;
        String endString;
        String payloadName[MAXINSTRUMENTS];
        String (* payloadGet [MAXINSTRUMENTS])();
        unsigned payloadLen;
};
