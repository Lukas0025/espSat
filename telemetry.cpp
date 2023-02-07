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

void Telemetry::addInstrument(String name, String (*payloadGet)()) {
  
  if (this->payloadLen >= MAXINSTRUMENTS) return;

  name.toUpperCase();

  this->payloadGet[this->payloadLen]  = payloadGet;
  this->payloadName[this->payloadLen] = name;

  this->payloadLen++;
}
