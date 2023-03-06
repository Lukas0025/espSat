/**
 * espSat project simple esp base satellite
 * main TOP file
 * @author Lukas Plevac <lukas@plevac.eu>
 */

#include "stateDrive.h"
#include "radio.h"
#include "debug.h"
#include "persistMem.h"
#include <WiFi.h>

SPIClass radioSPI(HSPI);
BluetoothSerial BTSerial;

RADIOHW radio = new Module(RADIO_NSS_PIN, RADIOLIB_NC, RADIO_RESET_PIN, RADIOLIB_NC, radioSPI);


void setup() {
  WiFi.persistent(false);             //disable saving wifi config into SDK flash area
  WiFi.mode(WIFI_OFF);                //disable swAP & station by calling "WiFi.mode(WIFI_OFF)" & put modem to sleep

  DEBUG_BEGIN();
  radioSPI.begin(INNER_SPI_SCLK, INNER_SPI_MISO, INNER_SPI_MOSI, RADIO_NSS_PIN);
  StateDrive::setup(&radio);
}

void loop() {
  //PersistMem::reset();
	StateDrive::run();
}
