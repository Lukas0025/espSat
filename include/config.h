/**
 * espSat project simple esp base satellite
 * File with satellite confogurations
 * @author Lukas Plevac <lukas@plevac.eu>
 */
#pragma once

#include "radio.h"
#include "pins.h"
#include <Arduino.h>
#include "esp_camera.h"

#define RTTY_ASCII 0                 // 7 data bits 
#define RTTY_ASCII_EXTENDED 1        // 8 data bits
#define RTTY_ITA2 RADIOLIB_ITA2      // old ita encoding

#define TELEMETRY_TYPE_UKHAS   0 //UKHAS Telemetry format like $$$$craftName,var,var,...,var*<checksum>\n
#define TELEMETRY_TYPE_RAWVARS 1 //Simple format          like $$$$ craftName :: varname:Var;varname:var ...

// I have 10k and 10k -> RATIO is 1/2 -> 2 is inverse
#define ADC_R_RATIO 2

// Max valtage what ADC can read
#define ADC_V_MAX   3.3

// Max number what ADC can return 12b here
#define ADC_BITS    4095

//Address of BMX280 device
#define BMX280_ADDR 118

// For calculate altudite
#define SEA_LEVEL_PRESSURE_HPA 1026.25

//#define ON_BOOT_RESET_PERSIST_MEM
//#define RESET_PERSIST_MEM_ONLY

namespace config {
  namespace camera {
    static camera_config_t RGB565 = {
      .pin_pwdn = CAM_PIN_PWDN,
      .pin_reset = CAM_PIN_RESET,
      .pin_xclk = CAM_PIN_XCLK,
      .pin_sccb_sda = CAM_PIN_SIOD,
      .pin_sccb_scl = CAM_PIN_SIOC,

      .pin_d7 = CAM_PIN_D7,
      .pin_d6 = CAM_PIN_D6,
      .pin_d5 = CAM_PIN_D5,
      .pin_d4 = CAM_PIN_D4,
      .pin_d3 = CAM_PIN_D3,
      .pin_d2 = CAM_PIN_D2,
      .pin_d1 = CAM_PIN_D1,
      .pin_d0 = CAM_PIN_D0,
      .pin_vsync = CAM_PIN_VSYNC,
      .pin_href = CAM_PIN_HREF,
      .pin_pclk = CAM_PIN_PCLK,

      //XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
      .xclk_freq_hz = 10000000,
      .ledc_timer = LEDC_TIMER_0,
      .ledc_channel = LEDC_CHANNEL_0,

      .pixel_format = PIXFORMAT_JPEG, //YUV422,GRAYSCALE,RGB565,JPEG
      .frame_size = FRAMESIZE_QVGA,    //QQVGA-UXGA, For ESP32, do not use sizes above QVGA when not JPEG. The performance of the ESP32-S series has improved a lot, but JPEG mode always gives better frame rates.

      .jpeg_quality = 6, //0-63, for OV series camera sensors, lower number means higher quality
      .fb_count = 1,       //When jpeg mode is used, if fb_count more than one, the driver will work in continuous mode.
      .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
    };

    static camera_config_t JpgQVGA = {
      .pin_pwdn = CAM_PIN_PWDN,
      .pin_reset = CAM_PIN_RESET,
      .pin_xclk = CAM_PIN_XCLK,
      .pin_sccb_sda = CAM_PIN_SIOD,
      .pin_sccb_scl = CAM_PIN_SIOC,

      .pin_d7 = CAM_PIN_D7,
      .pin_d6 = CAM_PIN_D6,
      .pin_d5 = CAM_PIN_D5,
      .pin_d4 = CAM_PIN_D4,
      .pin_d3 = CAM_PIN_D3,
      .pin_d2 = CAM_PIN_D2,
      .pin_d1 = CAM_PIN_D1,
      .pin_d0 = CAM_PIN_D0,
      .pin_vsync = CAM_PIN_VSYNC,
      .pin_href = CAM_PIN_HREF,
      .pin_pclk = CAM_PIN_PCLK,

      //XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
      .xclk_freq_hz = 10000000,
      .ledc_timer = LEDC_TIMER_0,
      .ledc_channel = LEDC_CHANNEL_0,

      .pixel_format = PIXFORMAT_JPEG, //YUV422,GRAYSCALE,RGB565,JPEG
      .frame_size = FRAMESIZE_QQVGA,    //QQVGA-UXGA, For ESP32, do not use sizes above QVGA when not JPEG. The performance of the ESP32-S series has improved a lot, but JPEG mode always gives better frame rates.

      .jpeg_quality = 63, //0-63, for OV series camera sensors, lower number means higher quality
      .fb_count = 1,       //When jpeg mode is used, if fb_count more than one, the driver will work in continuous mode.
      .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
    };

    static camera_config_t jpgHD = {
      .pin_pwdn = CAM_PIN_PWDN,
      .pin_reset = CAM_PIN_RESET,
      .pin_xclk = CAM_PIN_XCLK,
      .pin_sccb_sda = CAM_PIN_SIOD,
      .pin_sccb_scl = CAM_PIN_SIOC,

      .pin_d7 = CAM_PIN_D7,
      .pin_d6 = CAM_PIN_D6,
      .pin_d5 = CAM_PIN_D5,
      .pin_d4 = CAM_PIN_D4,
      .pin_d3 = CAM_PIN_D3,
      .pin_d2 = CAM_PIN_D2,
      .pin_d1 = CAM_PIN_D1,
      .pin_d0 = CAM_PIN_D0,
      .pin_vsync = CAM_PIN_VSYNC,
      .pin_href = CAM_PIN_HREF,
      .pin_pclk = CAM_PIN_PCLK,

      //XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
      .xclk_freq_hz = 10000000,
      .ledc_timer = LEDC_TIMER_0,
      .ledc_channel = LEDC_CHANNEL_0,

      .pixel_format = PIXFORMAT_JPEG, //YUV422,GRAYSCALE,RGB565,JPEG
      .frame_size = FRAMESIZE_HD,    //QQVGA-UXGA, For ESP32, do not use sizes above QVGA when not JPEG. The performance of the ESP32-S series has improved a lot, but JPEG mode always gives better frame rates.

      .jpeg_quality = 30, //0-63, for OV series camera sensors, lower number means higher quality
      .fb_count = 1,       //When jpeg mode is used, if fb_count more than one, the driver will work in continuous mode.
      .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
    };
  }

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
      .Shift          = 610,            // RTTY shift
      .Baud           = 50,             // Baud rate
      .Encoding       = RTTY_ASCII,     // Encoding ITA2
      .StopBits       = 2               // Number of stopbits 
    };

    const RTTYSettings_t rttyFast = {
      .Frequency      = 434.126,        // Base frequency
      .Shift          = 610,            // RTTY shift
      .Baud           = 100,            // Baud rate
      .Encoding       = RTTY_ASCII ,    // Encoding ITA2
      .StopBits       = 2               // Number of stopbits 
    };

    const SSTVSettings_t sstv = {
      .Frequency      = 434.126,        // Base frequency
      .Correction     = .981,
      .Mode           = Martin1
    };
  
    const LoraSettings_t loraTelemetry = {
      .Frequency      = 434.126,
      .Bandwidth      = 20.8,
      .SpreadFactor   = 11,
      .CodeRate       = 8,
      .SyncWord       = 0x12,
      .Power          = 17,
      .CurrentLimit   = 100,
      .PreambleLength = 8,
      .Gain           = 0
    };

    const LoraSettings_t loraSSDOFast = {
      .Frequency      = 434.126,
      .Bandwidth      = 250,
      .SpreadFactor   = 7,
      .CodeRate       = 6,
      .SyncWord       = 0x12,
      .Power          = 17,
      .CurrentLimit   = 100,
      .PreambleLength = 8,
      .Gain           = 0
    };

    const uint32_t craftIdLoraHDImg     = 0;
    const uint32_t craftIdLoraImg       = 1;
    const uint32_t craftIdLoraTelemetry = 2;
  }

  namespace telemetry {
    const String craftId     = "ESPCAMSAT-0001";
    const String beginString = "$$$$$$";
    const String endString   = "\n";
    const uint8_t type       = TELEMETRY_TYPE_UKHAS;
  }

  namespace sleep {
    const uint16_t timeToSleep = 5 * 60; //in S
  }
}
