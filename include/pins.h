/**
 * espSat project simple esp base satellite
 * File with satellite pins configurations
 * @author Lukas Plevac <lukas@plevac.eu>
 */
#pragma once

//general pins
#define INNER_SPI_MISO              15
#define INNER_SPI_MOSI              2
#define INNER_SPI_SCLK              14

//Radio pins
#define RADIO_NSS_PIN               13
#define RADIO_RESET_PIN             4

//Camera pins
#define CAM_PIN_PWDN                32
#define CAM_PIN_RESET               -1 //software reset will be performed
#define CAM_PIN_XCLK                0
#define CAM_PIN_SIOD                26
#define CAM_PIN_SIOC                27

#define CAM_PIN_D7                  35
#define CAM_PIN_D6                  34
#define CAM_PIN_D5                  39
#define CAM_PIN_D4                  36
#define CAM_PIN_D3                  21
#define CAM_PIN_D2                  19
#define CAM_PIN_D1                  18
#define CAM_PIN_D0                  5
#define CAM_PIN_VSYNC               25
#define CAM_PIN_HREF                23
#define CAM_PIN_PCLK                22
