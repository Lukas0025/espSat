# 1 "/tmp/tmp_vdr65kt"
#include <Arduino.h>
# 1 "/home/lukasplevac/Git/espSat/src/espSat.ino"






#include "stateDrive.h"
#include "radio.h"
#include "debug.h"
#include "persistMem.h"
#include <WiFi.h>

SPIClass radioSPI(HSPI);
BluetoothSerial BTSerial;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


RADIOHW radio = new Module(RADIO_NSS_PIN, RADIOLIB_NC, RADIO_RESET_PIN, RADIOLIB_NC, radioSPI);
void setup();
void loop();
#line 24 "/home/lukasplevac/Git/espSat/src/espSat.ino"
void setup() {
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);

  DEBUG_BEGIN();
  radioSPI.begin(INNER_SPI_SCLK, INNER_SPI_MISO, INNER_SPI_MOSI, RADIO_NSS_PIN);
  StateDrive::setup(&radio);
}

void loop() {

 StateDrive::run();
}