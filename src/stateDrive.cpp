#include "stateDrive.h"
#include <EEPROM.h>


#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  10       /* Time ESP32 will go to sleep (in seconds) */

namespace StateDrive {
	RADIOHW      *radio;
	RadioControl *radioControl;
	Telemetry    *telemetry;
	state_t (*currentState)() = initState;
	
	RTTYClient *rtty;
	SSTVClient *sstv;

	void run() {
		currentState();		
	}
	
	void setup(RADIOHW* radioSX) {     
		radio        = radioSX;
		radioControl = new RadioControl(radio);
		telemetry    = new Telemetry("ESPCAMSAT-0001", "?B?B?B?B ", " ?E?E?E?E");
		sstv         = new SSTVClient(radio);
		rtty         = new RTTYClient(radio);
		
		instruments::setup();
		instruments::autoAddToTelemetry(telemetry);
	}

	state_t initState() {
		DEBUG_PRINT("Sattelite is in init state. Current ALT is ", instruments::getAlt(), " and INIT ALT is ", INIT_ALT);

		if (instruments::getAlt() >= INIT_ALT) NEXT_HARD_STATE(idleState); 
		
		NEXT_SOFT_STATE(initState);
	}

	state_t idleState() {
		uint16_t transmitCounter = instruments::getTransmitCounter();

		DEBUG_PRINT("Sattelite is in IDLE state. Current transmitCounter is ", transmitCounter);

		//LoraTelemetry every 10min and other every 30min
		if (transmitCounter % 6 == 0) NEXT_SOFT_STATE(loraTelemetryState);
		if (transmitCounter % 6 == 1) NEXT_SOFT_STATE(rttyState);	
		if (transmitCounter % 6 == 2) NEXT_SOFT_STATE(loraTelemetryState);
		if (transmitCounter % 6 == 3) NEXT_SOFT_STATE(sstvState);
		if (transmitCounter % 6 == 4) NEXT_SOFT_STATE(loraTelemetryState);
		if (transmitCounter % 6 == 5) NEXT_SOFT_STATE(loraFastSSDOState);

		NEXT_SOFT_STATE(loraTelemetryState);
	}

	state_t rttyState() {
		//powerControl.powerOnBMP280();
		//powerControl.powerOnVoltmeter();
		//instruments::setupTelemetryInstruments();

		String state = telemetry->getState();

		//powerControl.powerOffBMP280();
		//powerControl.powerOffVoltmeter();
		//powerControl.powerOnRadio();

		radioControl->setupFSK(config::radio::fsk);
		radioControl->setupRTTY(config::radio::rttySlow, rtty);

		radioControl->sendRTTY(state);

		//powerControl.powerOffRadio();

		instruments::incTransmitCounter();
		NEXT_SOFT_STATE(sleepState);
	}

	state_t sstvState() {
		//powerControl.powerOnRadio();
		radioControl->setupFSK(config::radio::fsk);
		radioControl->setupSSTV(config::radio::sstv, sstv);
		radioControl->sendSSTV((uint16_t*)TestData::imageRGB565);
		//powerControl.powerOffRadio();

		instruments::incTransmitCounter();
		NEXT_SOFT_STATE(sleepState);
	}

	state_t loraFastSSDOState() {
		SSDO ssdoProtocol = SSDO(LORA_CRAFT_ID, instruments::incGetLoraCounter());

		//powerControl.powerOnRadio();
		radioControl->setupLora(config::radio::loraSSDVFast);

		uint8_t packet[SSDO_PACKET_SIZE];
		for (unsigned i = 0; i < ssdoProtocol.packetsCount(TestData::imageJPGHDSize); i++) { 
			unsigned packetLen = ssdoProtocol.setPacket((uint8_t*)TestData::imageJPGHD, i, packet, TestData::imageJPGHDSize);
			radioControl->sendLora(packet, packetLen);
		}

		//powerControl.powerOffRadio();

		instruments::incTransmitCounter();
		NEXT_SOFT_STATE(sleepState);
	}

	state_t loraSlowSSDOState() {
		SSDO ssdoProtocol = SSDO(LORA_CRAFT_ID, instruments::incGetLoraCounter());

		//powerControl.powerOnRadio();
		radioControl->setupLora(config::radio::loraTelemetry);

		uint8_t packet[SSDO_PACKET_SIZE];
		for (unsigned i = 0; i < 0; i++) { 
			unsigned packetLen = ssdoProtocol.setPacket((uint8_t*)TestData::imageJPGHD, i, packet, TestData::imageJPGHDSize);
			radioControl->sendLora(packet, packetLen);
		}

		//powerControl.powerOffRadio();

		NEXT_SOFT_STATE(sleepState);
	}

	state_t loraTelemetryState() {
		//powerControl.powerOnBMP280();
		//powerControl.powerOnVoltmeter();
		//instruments::setupTelemetryInstruments();

		String state = telemetry->getState();

		//powerControl.powerOffBMP280();
		//powerControl.powerOffVoltmeter();

    SSDO ssdoProtocol = SSDO(LORA_CRAFT_ID, instruments::incGetLoraCounter());
    
		//powerControl.powerOnRadio();

		radioControl->setupLora(config::radio::loraTelemetry);
		
		uint8_t packet[SSDO_PACKET_SIZE];
    for (unsigned i = 0; i < ssdoProtocol.packetsCount(state.length()); i++) { 
      unsigned packetLen = ssdoProtocol.setPacket((uint8_t*)state.c_str(), i, packet, state.length());
      radioControl->sendLora(packet, packetLen);
    }

		//powerControl.powerOffRadio();

		instruments::incTransmitCounter();
		NEXT_SOFT_STATE(loraSlowSSDOState);
	}

	state_t sleepState() {
		delay(10000);
		NEXT_SOFT_STATE(idleState);
	}
	
}
