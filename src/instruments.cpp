/**
 * espSat project simple esp base satellite
 * File with implementation of sensor manager
 * @author Lukas Plevac <lukas@plevac.eu>
 */

#include "instruments.h"
#include "persistMem.h"
#include "testData.h"

namespace instruments {

    void setup() {
      PersistMem::init();
    }

    /*
     * Functions for work with satellite sensors
     */

    unsigned getUptime() {
      return millis();
    }

    unsigned getExpectedRunTime() {
      return PersistMem::getTransmitCounter() * 30;
    }

    float getAlt() {
      return 1000;
    }

    float getPressure() {
      return 1100;
    }

    float getVoltage() {
      return 3;
    }

    float getTemperature() {
      return 10;
    }

    /*
     * Functions for work with satellite counters
     */

    uint32_t getTransmitCounter() {
      return PersistMem::getTransmitCounter();
    }

    uint32_t getLoraCounter() {
      return PersistMem::getTransmitCounter();
    }

    uint16_t getBootCounter() {
      return PersistMem::getBootCounter();
    }

    uint32_t incGetTransmitCounter() {
      auto tc = PersistMem::getTransmitCounter(); 
      PersistMem::setTransmitCounter(++tc);

      return tc;
    }

    uint32_t incGetLoraCounter() {
      auto lc = PersistMem::getLoraCounter(); 
      PersistMem::setLoraCounter(++lc);

      return lc;
    }

    uint16_t incGetBootCounter() {
      auto bc = PersistMem::getBootCounter(); 
      PersistMem::setLoraCounter(++bc);

      return bc;
    }

    camera_fb_t* cameraCaptureJpgQVGA() {
      esp_err_t err = esp_camera_init(&config::camera::JpgQVGA);

      if (err != ESP_OK) {
        ERROR_PRINT("Camera init failed with error ", err);
        return NULL;
      }

      vTaskDelay(500 / portTICK_RATE_MS);

      camera_fb_t *fb = esp_camera_fb_get();
      if (!fb) {
        ERROR_PRINT("Camera capture failed");
        return NULL;
      } 

      DEBUG_PRINT("Captured image Width:",  fb->width);
      DEBUG_PRINT("Captured image Height:", fb->height);
      DEBUG_PRINT("Captured image Format:", fb->format);
      DEBUG_PRINT("Captured image Lenght:", fb->len);

      return fb;
    }

    camera_fb_t* cameraCaptureJpgHD() {
      esp_err_t err = esp_camera_init(&config::camera::jpgHD);
      if (err != ESP_OK) {
        ERROR_PRINT("Camera init failed with error ", err);
        return NULL;
      }

      vTaskDelay(500 / portTICK_RATE_MS);

      camera_fb_t *fb = esp_camera_fb_get();
      if (!fb) {
        ERROR_PRINT("Camera capture failed");
        return NULL;
      } 

      DEBUG_PRINT("Captured image Width:",  fb->width);
      DEBUG_PRINT("Captured image Height:", fb->height);
      DEBUG_PRINT("Captured image Format:", fb->format);
      DEBUG_PRINT("Captured image Lenght:", fb->len);

      return fb;
    }

    void setCameraSettings() {
      sensor_t * s = esp_camera_sensor_get();
      s->set_brightness(s, 0);     // -2 to 2
      s->set_contrast(s, 0);       // -2 to 2
      s->set_saturation(s, 0);     // -2 to 2
      s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
      s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
      s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
      s->set_wb_mode(s, 0);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
      s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
      s->set_aec2(s, 0);           // 0 = disable , 1 = enable
      s->set_ae_level(s, 0);       // -2 to 2
      s->set_aec_value(s, 300);    // 0 to 1200
      s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
      s->set_agc_gain(s, 0);       // 0 to 30
      s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
      s->set_bpc(s, 0);            // 0 = disable , 1 = enable
      s->set_wpc(s, 1);            // 0 = disable , 1 = enable
      s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
      s->set_lenc(s, 1);           // 0 = disable , 1 = enable
      s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
      s->set_vflip(s, 0);          // 0 = disable , 1 = enable
      s->set_dcw(s, 1);            // 0 = disable , 1 = enable
      s->set_colorbar(s, 0);       // 0 = disable , 1 = enable
    }

    uint16_t* cameraCaptureRGB565() {
      
      esp_err_t err = esp_camera_init(&config::camera::RGB565);
      if (err != ESP_OK) {
        ERROR_PRINT("Camera init failed with error ", err);
        return (uint16_t*) TestData::imageRGB565;
      }

      instruments::setCameraSettings();

      camera_fb_t *fb = esp_camera_fb_get();
      if (!fb) {
        ERROR_PRINT("Camera capture failed");
        return (uint16_t*) TestData::imageRGB565;
      } 

      DEBUG_PRINT("Captured image Width:",  fb->width);
      DEBUG_PRINT("Captured image Height:", fb->height);
      DEBUG_PRINT("Captured image Format:", fb->format);
      DEBUG_PRINT("Captured image Lenght:", fb->len);

      auto psImageBuf =  (uint8_t*)ps_malloc(320*240*2);

      //convert JPG to RGB565
      jpg2rgb565(fb->buf, fb->len, psImageBuf, JPG_SCALE_NONE);

      //return the frame buffer back to the driver for reuse
    	esp_camera_fb_return(fb);

      return (uint16_t*) psImageBuf;
    }

    /*
     * This is telemetry support functions return state of senzor in String
     */

    String intAs2Str(unsigned val) {
      if (val > 99) val = val % 100;
      if (val < 10) return "0" + String(val);
      
      return String(val);
    }

    String uptimeStr()           { 
      return intAs2Str(getExpectedRunTime() / 3600)    + ":" + // Hour
             intAs2Str(getExpectedRunTime() / 60 % 60) + ":" + // Min
             intAs2Str(getExpectedRunTime() % 60);             // Sec
    }

    String latStr()              { return "0.0"; }
    String lonStr()              { return "0.0"; }
    String voltageStr()          { return String((int)(getVoltage()     * 100)); }
    String temperatureStr()      { return String((int)(getTemperature() * 10 )); }
    String altitudeStr()         { return String((int)getAlt()               );  }
    String pressureStr()         { return String((int)getPressure()          );  }
    String loraCounterStr()      { return String(getLoraCounter()            );  }
    String transmitCounterStr()  { return String(getTransmitCounter()        );  }
    String bootCounterStr()      { return String(getBootCounter()            );  }

    /*
     * Add standart instruments to telemetry
     */
    void autoAddToTelemetry(Telemetry *telemetry) {
      telemetry->addInstrument("TCOUNTER",    transmitCounterStr);
      telemetry->addInstrument("TIME",        uptimeStr);
      telemetry->addInstrument("LAT",         latStr);
      telemetry->addInstrument("LON",         lonStr);
      telemetry->addInstrument("ALT",         altitudeStr);
      telemetry->addInstrument("voltage",     voltageStr);
      telemetry->addInstrument("temperature", temperatureStr);
      telemetry->addInstrument("pressure",    pressureStr);
      telemetry->addInstrument("lcounter",    loraCounterStr);
      telemetry->addInstrument("bcounter",    bootCounterStr);
    }
}
