#include "radio.h"

bool RadioControl::setupRTTY(RTTYSettings_t RTTYSettings, RTTYClient *rtty) {
  
  printDebug("Stuping RTTY");

  if (!this->fskReady) {
    printDebug("ERROR fsk not setuped");
    return false;
  }

  this->rtty = rtty;
  
  int16_t state = this->rtty->begin(
    RTTYSettings.Frequency,
    RTTYSettings.Shift,
    RTTYSettings.Baud,
    RTTYSettings.Encoding,
    RTTYSettings.StopBits
  );
                     
  if(state != RADIOLIB_ERR_NONE) {
    printDebug2("Fail to setup RTTY. Error code: ", state);
    return false;
  }

  printDebug("RTTY setuped");
  
  this->rttyReady = true;
  return true;
}

bool RadioControl::setupFSK(FSKSettings_t FSKSettings) {

  printDebug("Setuping FSK");
 
  int16_t state = this->radio->beginFSK(
    FSKSettings.Frequency,
    FSKSettings.BitRate,
    FSKSettings.FreqDev,
    FSKSettings.RXBandwidth,
    FSKSettings.Power,
    FSKSettings.PreambleLength,
    FSKSettings.EnableOOK
  );


  if(state != RADIOLIB_ERR_NONE) {
    printDebug2("Fail to setup FSK. Error code: ", state);
    return false;
  }

  this->fskReady  = true;
  this->loraReady = false;
  printDebug("FSK setuped");
  
  return true;
}

bool RadioControl::setupSSTV(SSTVSettings_t ssvtsettings, SSTVClient *sstv) {
  
  int state = sstv->begin(ssvtsettings.Frequency, ssvtsettings.Mode);
  if(state != RADIOLIB_ERR_NONE) {
    printDebug2("Fail to setup SSTV. Error code: ", state);
    return false;
  }

  // set correction factor
  // NOTE: Due to different speeds of various platforms
  //       supported by RadioLib (Arduino Uno, ESP32 etc),
  //       and because SSTV is analog protocol, incorrect
  //       timing of pulses can lead to distortions.
  //       To compensate, correction factor can be used
  //       to adjust the length of timing pulses
  //       (lower number = shorter pulses).
  //       The value is usually around 0.95 (95%).
  state = sstv->setCorrection(ssvtsettings.Correction);
  if(state != RADIOLIB_ERR_NONE) {
    printDebug2("Fail to set SSTV Correction. Error code: ", state);
    return false;
  }

  this->sstvReady  = true;
  this->loraReady  = false;
  this->fskReady   = false;
  this->rttyReady  = false;

  this->sstv = sstv;

  printDebug("SSTV stuped");

  return true;
}

bool RadioControl::setupLora(LoraSettings_t LoRaSettings) {
  
  printDebug("Setuping LORA");

  int16_t state = this->radio->begin(
    LoRaSettings.Frequency,
    LoRaSettings.Bandwidth,
    LoRaSettings.SpreadFactor,
    LoRaSettings.CodeRate,
    LoRaSettings.SyncWord,
    LoRaSettings.Power,
    LoRaSettings.PreambleLength, 
    LoRaSettings.Gain
  );
  
  this->radio->setCRC(true);
  
  if(state != RADIOLIB_ERR_NONE) {
    printDebug2("Fail to setup LORA. Error code: ", state);
    return false;
  }

  this->loraReady  = true;
  this->fskReady   = false;
  this->rttyReady  = false;
  this->sstvReady  = false;
  printDebug("LORA setuped");
  
  return true;
}

bool RadioControl::sendRTTY(String message) {

  if (!this->rttyReady) {
    printDebug("ERROR rtty not setuped");
    return false;
  }
  
  printDebug("rtty IDLE");
  this->rtty->idle();     

  delay(RTTY_IDLE_TIME); 

  printDebug2("rtty sending ", message); 
  this->rtty->println(message);
  this->rtty->standby();

  return true;
}

bool RadioControl::sendLora(uint8_t* message, unsigned size) {
  if (!this->loraReady) {
    printDebug("ERROR lora not setuped");
    return false;
  }
  
  this->radio->transmit(message, size);
  
  return true;
}

bool RadioControl::sendSSTV(uint16_t *image) {

  if (!this->sstvReady) {
    printDebug("ERROR sstv not setuped");
    return false;
  }

  this->sstv->idle();
  delay(10000);
  
  printDebug("sstv set headers");
  this->sstv->sendHeader();  

  printDebug("sstv sending"); 
  for(uint8_t i = 0; i < 240; i++) {
    uint32_t line[320];

    //parse line
    for (uint16_t j = 0; j < 320; j++) {
      uint8_t r = (image[i * 320 + j] >> 11) & 0x1F;  
      uint8_t g = (image[i * 320 + j] >> 5)  & 0x3F;
      uint8_t b =  image[i * 320 + j]        & 0x1F;

      r = (r * 255 + 15) / 31;
      b = (b * 255 + 15) / 31;
      g = (g * 255 + 31) / 63;
      
      line[j] = (r << 16) | (g << 8) | b;
    }
    
    this->sstv->sendLine(line);
  }

  // turn off transmitter
  this->radio->standby(); 

  return true;
}
