/**
 * espSat project simple esp base satellite
 * File with debug print macros
 * @author Lukas Plevac <lukas@plevac.eu>
 */
#pragma once
#include <Arduino.h>
#include "BluetoothSerial.h"

//comment out for no debug
#define DEBUG

//#define DEBUG_STREAM Serial
#define DEBUG_STREAM BTSerial

#if DEBUG_STREAM == BTSerial 
  extern BluetoothSerial BTSerial;

  #define DEBUG_STREAM_PRINT(X)   DEBUG_STREAM.write((const uint8_t*)String(X).c_str(),          strlen(String(X).c_str()) + 1)
  #define DEBUG_STREAM_PRINTLN(X) DEBUG_STREAM.write((const uint8_t*)(String(X) + "\n").c_str(), strlen(String(X).c_str()) + 2)
#elif DEBUG_STREAM == Serial 
  #define DEBUG_STREAM_PRINT(X)   DEBUG_STREAM.print(X)
  #define DEBUG_STREAM_PRINTLN(X) DEBUG_STREAM.println(X)
#endif

//for set DEBUG LEVEL 0 for show all 1 for WARNING and ERROR and 2 only for ERROR
#define DEBUG_LEVEL 0 

/**
 * Macros to print string with N params UPTO 5
 */
#define ARGS_PRINT_1(X)      DEBUG_STREAM_PRINT(X) 
#define ARGS_PRINT_2(X, ...) DEBUG_STREAM_PRINT(X); ARGS_PRINT_1(__VA_ARGS__)
#define ARGS_PRINT_3(X, ...) DEBUG_STREAM_PRINT(X); ARGS_PRINT_2(__VA_ARGS__)
#define ARGS_PRINT_4(X, ...) DEBUG_STREAM_PRINT(X); ARGS_PRINT_3(__VA_ARGS__)
#define ARGS_PRINT_5(X, ...) DEBUG_STREAM_PRINT(X); ARGS_PRINT_4(__VA_ARGS__)
//... repeat as needed

/**
 * Get print macro by number of parameters __VA_ARGS__ do params shift
 * EXAMPLE if __VA_ARGS__ is ("HELLO", "WORLD") then NAME is ARGS_PRINT_2
 * 
 *     _1       _2          _3             _4            _5              NAME           ...
 * ("HELLO", "WORLD",  ARGS_PRINT_5,  ARGS_PRINT_4,  ARGS_PRINT_3,  ARGS_PRINT_2,  ARGS_PRINT_1)
 * 
 * @param _1 fixed   to __VA_ARGS__
 * @param _2 fixed   to ARGS_PRINT_5
 * @param _3 fixed   to ARGS_PRINT_4
 * @param _4 fixed   to ARGS_PRINT_3
 * @param _5 fixed   to ARGS_PRINT_2
 * @param X  fixed   to ARGS_PRINT_1 (Macro to use)
 * @param ... other parameters
 * @return name of macro to use
 */
#define GET_PRINT_MACRO(_1, _2, _3, _4, _5, X, ...) X 

/**
 * Print n string arguments to serial line
 * @param ... string parametrs to print UPTO 5
 */
#define FOR_EACH_PRINT(...) GET_PRINT_MACRO(__VA_ARGS__,  ARGS_PRINT_5,  ARGS_PRINT_4,  ARGS_PRINT_3,  ARGS_PRINT_2,  ARGS_PRINT_1)(__VA_ARGS__)

// if macro debug defined
#ifdef DEBUG
  const char HexTable[] = "0123456789ABCDEF";

  #if DEBUG_STREAM == BTSerial 
    /**
     * Open serial line for debug
     */
    #define DEBUG_BEGIN() { \
      BTSerial.begin("ESPSATDebug");   \
      delay(10000);         \
    }
  #elif DEBUG_STREAM == Serial 
    /**
     * Open serial line for debug
     */
    #define DEBUG_BEGIN() { \
      Serial.begin(9600);   \
      delay(10000);         \
    }
  #endif

  #define DEBUG_PRINT_HEX(DATA, LEN) {                 \
    DEBUG_STREAM_PRINTLN("");                                \
    for (unsigned i = 0; i < LEN; i++) {               \
      DEBUG_STREAM_PRINT(HexTable[(DATA[i] >> 4) & 0b1111]); \
      DEBUG_STREAM_PRINT(HexTable[DATA[i]        & 0b1111]); \
    }                                                  \
    DEBUG_STREAM_PRINTLN("");                                \
  }
  
  /**
   * Print location in program to serial line
   */
  #define DEBUG_PRINT_CODE_LOC() { \
    DEBUG_STREAM_PRINT(__FILE__);        \
    DEBUG_STREAM_PRINT(":");             \
    DEBUG_STREAM_PRINT(__LINE__);        \
    DEBUG_STREAM_PRINT(":");             \
    DEBUG_STREAM_PRINT(__func__);        \
    DEBUG_STREAM_PRINT("(): ");          \
  }

  /**
   * Print DEBUG message to serial line with
   * program location 
   * @param ... multiple information to print UPTO by FOR_EACH_PRINT
   */
  #define DEBUG_PRINT(...) {       \
    if (DEBUG_LEVEL <= 0) {        \
      DEBUG_STREAM_PRINT("[DEBUG]   ");  \
      DEBUG_PRINT_CODE_LOC();      \
      FOR_EACH_PRINT(__VA_ARGS__); \
      DEBUG_STREAM_PRINTLN("");          \
    }                              \
  }

  /**
   * Print WARNING message to serial line with
   * program location 
   * @param ... multiple information to print UPTO by FOR_EACH_PRINT
   */
  #define WARNING_PRINT(...) {   \
    DEBUG_STREAM_PRINT("[WARNING] ");  \
    DEBUG_PRINT_CODE_LOC();      \
    FOR_EACH_PRINT(__VA_ARGS__); \
    DEBUG_STREAM_PRINTLN("");          \
  }

  /**
   * Print ERROR message to serial line with
   * program location 
   * @param ... multiple information to print UPTO by FOR_EACH_PRINT
   */
  #define ERROR_PRINT(...) {     \
    DEBUG_STREAM_PRINT("[ERROR]   ");  \
    DEBUG_PRINT_CODE_LOC();      \
    FOR_EACH_PRINT(__VA_ARGS__); \
    DEBUG_STREAM_PRINTLN("");          \
  }

#else // if debug is desibled do nothing
  #define DEBUG_BEGIN()          {}
  #define DEBUG_PRINT_CODE_LOC() {}
  #define DEBUG_PRINT(...)       {}
  #define WARNING_PRINT(...)     {}
  #define ERROR_PRINT(...)       {}
  #define DEBUG_PRINT_HEX(DATA, LEN) {}
#endif
