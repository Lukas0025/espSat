/**
 * espSat project simple esp base satellite
 * File with implementation of radio manager
 * @author Lukas Plevac <lukas@plevac.eu>
 */
#include "radio.h"

bool RadioControl::setupRTTY(RTTYSettings_t RTTYSettings, RTTYClient *rtty) {
  
  DEBUG_PRINT("Stuping RTTY");

  if (!this->fskReady) {
    ERROR_PRINT("fsk not setuped");
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
    ERROR_PRINT("Fail to setup RTTY. Error code: ", state);
    return false;
  }

  DEBUG_PRINT("RTTY setuped");
  
  this->rttyReady = true;
  return true;
}

bool RadioControl::setupFSK(FSKSettings_t FSKSettings) {

  DEBUG_PRINT("Setuping FSK");
  delay(1000);
  DEBUG_PRINT("Setuping FSK");
 
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
    ERROR_PRINT("Fail to setup FSK. Error code: ", state);
    return false;
  }

  this->fskReady  = true;
  this->loraReady = false;
  DEBUG_PRINT("FSK setuped");
  
  return true;
}

bool RadioControl::setupSSTV(SSTVSettings_t ssvtsettings, SSTVClient *sstv) {

  DEBUG_PRINT("Setuping SSTV");
  
  int state = sstv->begin(ssvtsettings.Frequency, ssvtsettings.Mode);

  if(state != RADIOLIB_ERR_NONE) {
    ERROR_PRINT("Fail to setup SSTV. Error code: ", state);
    return false;
  }

  state = sstv->setCorrection(ssvtsettings.Correction);
  
  if(state != RADIOLIB_ERR_NONE) {
    ERROR_PRINT("Fail to set SSTV Correction. Error code: ", state);
    return false;
  }

  this->sstvReady  = true;
  this->loraReady  = false;
  this->fskReady   = false;
  this->rttyReady  = false;

  this->sstv = sstv;

  DEBUG_PRINT("SSTV stuped");

  return true;
}

bool RadioControl::setupLora(LoraSettings_t LoRaSettings) {
  
  DEBUG_PRINT("Setuping LORA");

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
    ERROR_PRINT("Fail to setup LORA. Error code: ", state);
    return false;
  }

  this->loraReady  = true;
  this->fskReady   = false;
  this->rttyReady  = false;
  this->sstvReady  = false;
  DEBUG_PRINT("LORA setuped");
  
  return true;
}

bool RadioControl::sendRTTY(String message) {

  if (!this->rttyReady) {
    ERROR_PRINT("rtty not setuped");
    return false;
  }
  
  DEBUG_PRINT("rtty IDLE");
  this->rtty->idle();     

  delay(RTTY_IDLE_TIME); 

  DEBUG_PRINT("rtty sending ", message); 
  this->rtty->println(message);
  
  // turn off transmitter
  this->radio->standby(); 

  return true;
}

bool RadioControl::sendLora(uint8_t* message, unsigned size) {
  if (!this->loraReady) {
    ERROR_PRINT("ERROR lora not setuped");
    return false;
  }
  
  this->radio->transmit(message, size);

  // turn off transmitter
  //this->radio->standby(); 
  
  return true;
}

bool RadioControl::sendSSTV(uint16_t *image) {

  if (!this->sstvReady) {
    ERROR_PRINT("ERROR sstv not setuped");
    return false;
  }

  DEBUG_PRINT("sstv send sync tone");
  this->sstv->idle();
  delay(10000);
  
  DEBUG_PRINT("sstv set headers");
  this->sstv->sendHeader();  

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
    
    DEBUG_PRINT("sstv sending line ", i); 
    this->sstv->sendLine(line);
  }

  // turn off transmitter
  this->radio->standby(); 

  return true;
}

bool RadioControl::sendSSTVGS(uint8_t *image) {

  if (!this->sstvReady) {
    ERROR_PRINT("ERROR sstv not setuped");
    return false;
  }

  DEBUG_PRINT("sstv send sync tone");
  this->sstv->idle();
  delay(10000);
  
  DEBUG_PRINT("sstv set headers");
  this->sstv->sendHeader();  

  for(uint8_t i = 0; i < 240; i++) {
    uint32_t line[320];

    //parse line
    for (uint16_t j = 0; j < 320; j++) {
      line[j] = (image[j] << 16) | (image[j] << 8) | image[j];
    }
    
    DEBUG_PRINT("sstv sending line ", i); 
    this->sstv->sendLine(line);
  }

  // turn off transmitter
  this->radio->standby(); 

  return true;
}
