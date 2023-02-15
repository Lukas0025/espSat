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

char Hex(char Character)
{
  char HexTable[] = "0123456789ABCDEF";
  
  return HexTable[Character];
}

String Telemetry::getSonde() {
   char Sen[5];
   int Count, i, j;
   unsigned char c;
   unsigned int CRC, xPolynomial;
   char LatitudeString[16] = "53.163067", LongitudeString[16] = "6.16444", CRCString[8];
   char InternalTemp[10] = "10.20";
   char BattVoltage[10] = "3.33";
   char ExtVoltage[10] = "0.2";  

   char Sentence[255];
   
   sprintf(Sentence, "%s%s,%ld,%02d:%02d:%02d,%s,%s,%ld,%u,%u,%s,%s",
      "$$",
      "RTTY2",
      10,
      0, 0, 1,   
      LatitudeString,
      LongitudeString,
      1000,
      0,
      0,
      InternalTemp,
      BattVoltage         
  );

   Count = strlen(Sentence);

   // Calc CRC
   CRC = 0xffff;           // Seed
   xPolynomial = 0x1021;
   
   for (i = strlen("$$"); i < Count; i++)
   {   // For speed, repeat calculation instead of looping for each bit
      CRC ^= (((unsigned int)Sentence[i]) << 8);
      for (j=0; j<8; j++)
      {
          if (CRC & 0x8000)
              CRC = (CRC << 1) ^ 0x1021;
          else
              CRC <<= 1;
      }
   }

   Sentence[Count++] = '*';
   Sentence[Count++] = Hex((CRC >> 12) & 15);
   Sentence[Count++] = Hex((CRC >> 8) & 15);
   Sentence[Count++] = Hex((CRC >> 4) & 15);
   Sentence[Count++] = Hex(CRC & 15);
   Sentence[Count++] = '\n';  
   Sentence[Count++] = '\0';

   return Sentence;
}

void Telemetry::addInstrument(String name, String (*payloadGet)()) {
  
  if (this->payloadLen >= MAXINSTRUMENTS) return;

  name.toUpperCase();

  this->payloadGet[this->payloadLen]  = payloadGet;
  this->payloadName[this->payloadLen] = name;

  this->payloadLen++;
}
