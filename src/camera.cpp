#include "image.h"
#include "camera.h"
#include "debug.h"

namespace camera {
    void setCameraSettings() {
      // Rotate some frames to get parametres of enviromet and auto tune parametres
      for (int i = 0; i < 10; i++) {
        auto fb = esp_camera_fb_get();   
        esp_camera_fb_return(fb);
      }
    }

    image::image_t captureRGB565(camera_config_t *config) {
        esp_err_t err = esp_camera_init(config);
        image::image_t res;
        
        if (err != ESP_OK) {
            ERROR_PRINT("Camera init failed with error ", err);
            return res;
        }
        
        setCameraSettings();
        
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
        
        image::image_t rgb565;
        rgb565.buf    = (uint8_t*)image::_malloc(fb->width * fb->height * 2);
        rgb565.len    = fb->width * fb->height * 2;
        rgb565.fromat = PIXFORMAT_RGB565;
        rgb565.height = fb->height;
        rgb565.width  = fb->width;

        //convert JPG to RGB565
        jpg2rgb565(fb->buf, fb->len, rgb565.buf, JPG_SCALE_NONE);

        esp_camera_fb_return(fb);

        return rgb565;
    }

    image::image_t captureJpg(camera_config_t *config, uint8_t quality) {
        auto rgb565 = captureRGB565(config);
        auto jpg    = image::jpg(rgb565, quality);

        image::release(rgb565);

        return jpg;
    }
}
