/**
 * File with debug print macros
 * @author Lukas Plevac <lukas@plevac.eu>
 */
#pragma once
#include <Arduino.h>

//comment out for no debug
#define DEBUG

//for set DEBUG LEVEL 0 for show all 1 for WARNING and ERROR and 2 only for ERROR
#define DEBUG_LEVEL 0 

/**
 * Macros to print string with N params UPTO 5
 */
#define ARGS_PRINT_1(X)      Serial.print(X) 
#define ARGS_PRINT_2(X, ...) Serial.print(X); ARGS_PRINT_1(__VA_ARGS__)
#define ARGS_PRINT_3(X, ...) Serial.print(X); ARGS_PRINT_2(__VA_ARGS__)
#define ARGS_PRINT_4(X, ...) Serial.print(X); ARGS_PRINT_3(__VA_ARGS__)
#define ARGS_PRINT_5(X, ...) Serial.print(X); ARGS_PRINT_4(__VA_ARGS__)
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

  /**
   * Open serial line for debug
   */
  #define DEBUG_BEGIN() { \
    Serial.begin(9600); \
  }
  
  /**
   * Print location in program to serial line
   */
  #define DEBUG_PRINT_CODE_LOC() { \
    Serial.print(__FILE__);        \
    Serial.print(":");             \
    Serial.print(__LINE__);        \
    Serial.print(":");             \
    Serial.print(__func__);        \
    Serial.print("(): ");          \
  }

  /**
   * Print DEBUG message to serial line with
   * program location 
   * @param ... multiple information to print UPTO by FOR_EACH_PRINT
   */
  #define DEBUG_PRINT(...) {       \
    if (DEBUG_LEVEL <= 0) {        \
      Serial.print("[DEBUG]   ");  \
      DEBUG_PRINT_CODE_LOC();      \
      FOR_EACH_PRINT(__VA_ARGS__); \
      Serial.println("");          \
    }                              \
  }

  /**
   * Print WARNING message to serial line with
   * program location 
   * @param ... multiple information to print UPTO by FOR_EACH_PRINT
   */
  #define WARNING_PRINT(...) {   \
    Serial.print("[WARNING] ");  \
    DEBUG_PRINT_CODE_LOC();      \
    FOR_EACH_PRINT(__VA_ARGS__); \
    Serial.println("");          \
  }

  /**
   * Print ERROR message to serial line with
   * program location 
   * @param ... multiple information to print UPTO by FOR_EACH_PRINT
   */
  #define ERROR_PRINT(...) {     \
    Serial.print("[ERROR]   ");  \
    DEBUG_PRINT_CODE_LOC();      \
    FOR_EACH_PRINT(__VA_ARGS__); \
    Serial.println("");          \
  }

#else // if debug is desibled do nothing
  #define DEBUG_BEGIN()          {}
  #define DEBUG_PRINT_CODE_LOC() {}
  #define DEBUG_PRINT(X)         {}
  #define WARNING_PRINT(X)       {}
  #define ERROR_PRINT(X)         {}
#endif
