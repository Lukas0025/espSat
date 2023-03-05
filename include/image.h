#pragma once
#include <Arduino.h>
#include <esp_camera.h>
#include "JPEGENC.h"

namespace image {
    typedef struct {
        uint8_t *buf    = NULL;
        size_t   len    = 0;
        unsigned width  = 0;
        unsigned height = 0;
        pixformat_t fromat;
    } image_t;
    

    image_t jpg(image_t image, uint8_t quality);
    unsigned espcam2jpegencPixT(pixformat_t pix);
    void *_malloc(size_t size);
    void release(image_t image);
}
