#pragma once

#include "radio.h"

#define NSS_PIN 1
#define DIO0_PIN 2
#define DIO1_PIN 3

#define RTTY_ASCII 0                 // 7 data bits 
#define RTTY_ASCII_EXTENDED 1        // 8 data bits

namespace config {
  namespace radio {
    const FSKSettings_t fsk = {
      .Frequency      = 434.126,
      .BitRate        = 44.0,
      .FreqDev        = 50.0,
      .RXBandwidth    = 125.0,
      .Power          = 17, // in dBm between 2 and 17. 10 = 10mW (recommended). Sets also RTTY power
      .PreambleLength = 16,
      .EnableOOK      = false,
      .dataShaping    = 0.5
    };
    
    const RTTYSettings_t rttySlow = {
      .Frequency      = 434.126,        // Base frequency
      .Shift          = 170,            // RTTY shift
      .Baud           = 44,             // Baud rate
      .Encoding       = RADIOLIB_ITA2,  // Encoding ITA2
      .StopBits       = 2               // Number of stopbits 
    };

    const RTTYSettings_t rttyFast = {
      .Frequency      = 434.126,        // Base frequency
      .Shift          = 850,            // RTTY shift
      .Baud           = 75,             // Baud rate
      .Encoding       = RADIOLIB_ITA2,  // Encoding ITA2
      .StopBits       = 2               // Number of stopbits 
    };

    const SSTVSettings_t sstv = {
      .Frequency      = 434.126,        // Base frequency
      .Correction     = .98,
      .Mode           = Martin1
    };
  
    const LoraSettings_t loraTelemetry = {
      .Frequency      = 434.126,
      .Bandwidth      = 20.8,
      .SpreadFactor   = 11,
      .CodeRate       = 8,
      .SyncWord       = 0x12,
      .Power          = 10,
      .CurrentLimit   = 100,
      .PreambleLength = 8,
      .Gain           = 0
    };

    const LoraSettings_t loraSSDVFast = {
      .Frequency      = 434.126,
      .Bandwidth      = 250,
      .SpreadFactor   = 7,
      .CodeRate       = 6,
      .SyncWord       = 0x12,
      .Power          = 10,
      .CurrentLimit   = 100,
      .PreambleLength = 8,
      .Gain           = 0
    };
  }

}
