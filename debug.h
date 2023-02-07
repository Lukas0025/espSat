#pragma once
#include <Arduino.h>

#define DEBUG

#ifdef DEBUG
  #define DEBUG_BEGIN() Serial.begin(9600)
  #define printDebugLocation() { Serial.print(__FILE__); Serial.print(":"); Serial.print(__LINE__); Serial.print(":"); Serial.print(__func__); Serial.print("(): "); }
  #define printDebug(X) { printDebugLocation(); Serial.println(X); }
  #define printDebug2(X, Y) { printDebugLocation(); Serial.print(X); Serial.println(Y); }
#else
  #define DEBUG_BEGIN()
  #define printDebugLocation()
  #define printDebug(X)
  #define printDebug2(X, Y)
#endif
