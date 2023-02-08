#include "include/stateDrive.h"
#include "include/radio.h"

#define RADIO_SCLK_PIN              5
#define RADIO_MISO_PIN              19
#define RADIO_MOSI_PIN              27
#define RADIO_CS_PIN                18
#define RADIO_DIO0_PIN              26
#define RADIO_RST_PIN               23
#define RADIO_DIO1_PIN              33
#define RADIO_DIO2_PIN              14
#define RADIO_BUSY_PIN              32

//RADIOHW        radio        = new Module(RADIO_CS_PIN, RADIO_DIO0_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);
RADIOHW radio        = new Module(RADIO_CS_PIN, RADIO_DIO0_PIN, RADIO_DIO1_PIN);

void setup() {
  Serial.begin(9600);
  StateDrive::setup(&radio);
}

void loop() {
	StateDrive::run();
}