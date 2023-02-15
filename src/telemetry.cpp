/**
 * espSat project simple esp base satellite
 * File with implementation of telemetry manager
 * @author Lukas Plevac <lukas@plevac.eu>
 */

#include "telemetry.h"

String Telemetry::getState() {
  String message = this->beginString + " " + this->craftName + " :: ";

  for (unsigned i = 0; i < this->payloadLen; i++) {
    String instrumentOut = this->payloadGet[i]();
    instrumentOut.toUpperCase();
    
    message += this->payloadName[i] + ":" + instrumentOut + ";";
  }

  message += this->endString;

  return message;
}

/**
 * Get hex number for 16bits
 */
String Hex(uint16_t Character) {
  char HexTable[] = "0123456789ABCDEF";
  String hexCode  = "";

  for (int i = 12; i >= 0; i -= 4) {
    hexcode += HexTable[(Character >> i) & 0b1111];
  }

  return hexCode;
}

uint16_t Telemetry::getUKHASCRC(String message) {
  uint16_t CRC         = 0xffff; // Seed
  uint16_t xPolynomial = 0x1021;
   
  for (unsigned i = strlen(this->beginString); i < strlen(message); i++) {
    CRC ^= (((uint8_t) message[i]) << 8);
    for (j=0; j<8; j++) {
      if (CRC & 0x8000) CRC = (CRC << 1) ^ 0x1021;
      else CRC <<= 1;
    }
  }

  return crc;
}

bool Telemetry::verifyUKHAS() {
  
  if (this->payloadLen < 5) return false;

  String expectedData[] = {
    "TCOUNTER",
    "TIME",
    "LAT",
    "LON",
    "ALT"
  }

  for (unsigned i = 0; i < 5; i++) {
    if (this->payloadName[i] != expectedData) return false;
  }

  return true;
}

String Telemetry::getUKHAS() {
  String message = this->beginString + this->craftName;

  if (!this->verifyUKHAS()) {
    DEBUG_WARNING("unrecomended order of data for UKHAS recomended is <TCOUNTER>,<TIME>,<LAT>,<LON>,<ALT>,<OTHER DATA>,...,<OTHER DATA>");
  }

  for (unsigned i = 0; i < this->payloadLen; i++) {
    String instrumentOut = this->payloadGet[i]();
    instrumentOut.toUpperCase();
    
    message += "," + instrumentOut;
  }

  return '*' + Hex(this->getUKHASCRC(message)) + this->endString;
}

void Telemetry::addInstrument(String name, String (*payloadGet)()) {
  
  if (this->payloadLen >= MAXINSTRUMENTS) return;

  name.toUpperCase();

  this->payloadGet[this->payloadLen]  = payloadGet;
  this->payloadName[this->payloadLen] = name;

  this->payloadLen++;
}
