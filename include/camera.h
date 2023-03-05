#pragma once
#include <Arduino.h>
#include <esp_camera.h>
#include "image.h"
#include "config.h"

namespace camera {
    image::image_t captureRGB565(camera_config_t *config);
    image::image_t captureJpg(camera_config_t *config, uint8_t quality);
}
