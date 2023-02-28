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

    void setCameraSettings() {
      // Rotate some frames to get parametres of enviromet and auto tune parametres
      for (int i = 0; i < 10; i++) {
        auto fb = esp_camera_fb_get();   
        esp_camera_fb_return(fb);
      }
    }

    camera_fb_t* cameraCaptureJpgQVGA() {
      esp_err_t err = esp_camera_init(&config::camera::JpgQVGA);

      if (err != ESP_OK) {
        ERROR_PRINT("Camera init failed with error ", err);
        return NULL;
      }

      instruments::setCameraSettings();

      delay(500);

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

    JpgRst::buffer_t cameraCaptureJpgHD() {
      esp_err_t err = esp_camera_init(&config::camera::jpgHD);
      JpgRst::buffer_t res;
      
      if (err != ESP_OK) {
        ERROR_PRINT("Camera init failed with error ", err);
        return res;
      }

      instruments::setCameraSettings();

      delay(500);

      camera_fb_t *fb = esp_camera_fb_get();
      if (!fb) {
        ERROR_PRINT("Camera capture failed");
        return res;
      } 

      DEBUG_PRINT("Captured image Width:",  fb->width);
      DEBUG_PRINT("Captured image Height:", fb->height);
      DEBUG_PRINT("Captured image Format:", fb->format);
      DEBUG_PRINT("Captured image Lenght:", fb->len);

      auto psImageBuf =  (uint8_t*)ps_malloc(fb->width * fb->height * 2);

      //convert JPG to RGB565
      jpg2rgb565(fb->buf, fb->len, psImageBuf, JPG_SCALE_NONE);

      //convert back to JPG with RST markers
      res = JpgRst::rgb5652jpg(psImageBuf, fb->width * fb->height * 2, fb->width, fb->height, 10);
      //fmt2jpg(psImageBuf, fb->width * fb->height * 2, fb->width, fb->height,  PIXFORMAT_RGB565, 30, &(res.data), &(res.len));


      esp_camera_fb_return(fb);

      return res;
    }

    uint16_t* cameraCaptureRGB565() {
      
      esp_err_t err = esp_camera_init(&config::camera::RGB565);
      if (err != ESP_OK) {
        ERROR_PRINT("Camera init failed with error ", err);
        return (uint16_t*) TestData::imageRGB565;
      }

      instruments::setCameraSettings();

      delay(500);

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
