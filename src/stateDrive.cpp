/**
 * espSat project simple esp base satellite
 * File with implementation of state machine of statellite
 * @author Lukas Plevac <lukas@plevac.eu>
 */
#include "stateDrive.h"
#include "persistMem.h"
#include "camera.h"

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

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
		telemetry    = new Telemetry(config::telemetry::craftId, config::telemetry::beginString, config::telemetry::endString);
		sstv         = new SSTVClient(radio);
		rtty         = new RTTYClient(radio);

		DEBUG_PRINT("Total heap: ", ESP.getHeapSize());
		DEBUG_PRINT("Free  heap: ", ESP.getFreeHeap());
		DEBUG_PRINT("Total PSRAM: ", ESP.getPsramSize());
		DEBUG_PRINT("Free  PSRAM: ", ESP.getFreePsram());

		//init persisten memory
		instruments::setup();

		esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

		if (ESP_SLEEP_WAKEUP_TIMER != wakeup_reason) { //detect type of wake up 
			#ifdef ON_BOOT_RESET_PERSIST_MEM
				PersistMem::reset();
				#ifdef RESET_PERSIST_MEM_ONLY
					while(true) {}
				#endif
			#endif

			instruments::incGetBootCounter();
		}

		//recover state
		currentState = PersistMem::getLastState();

		//configure wakeup alarm clock
		esp_sleep_enable_timer_wakeup(config::sleep::timeToSleep * uS_TO_S_FACTOR);

		//setup telemetry and instrments
		instruments::autoAddToTelemetry(telemetry);

		DEBUG_PRINT("Sattelite setup done");
	}

	state_t initState() {
		DEBUG_PRINT("Sattelite is in init state. Current ALT is ", instruments::getAlt(), " and INIT ALT is ", INIT_ALT);

		if (instruments::getAlt() >= INIT_ALT) NEXT_HARD_STATE(idleState); 
		
		NEXT_SOFT_STATE(initState);
	}

	state_t idleState() {
		uint32_t transmitCounter = instruments::getTransmitCounter();

		DEBUG_PRINT("Sattelite is in IDLE state. Current transmitCounter is ", transmitCounter);

		//increment transmit counter to prevent deadLock loops when same state fail
		instruments::incGetTransmitCounter();

		NEXT_SOFT_STATE(loraFastSSDOState);

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
		
		String state = "";
		if (config::telemetry::type == TELEMETRY_TYPE_RAWVARS) state = telemetry->getState();
		if (config::telemetry::type == TELEMETRY_TYPE_UKHAS)   state = telemetry->getUKHAS();

		radioControl->setupFSK(config::radio::fsk);
		radioControl->setupRTTY(config::radio::rttySlow, rtty);

		radioControl->sendRTTY(state);

		NEXT_SOFT_STATE(sleepState);
	}

	state_t sstvState() {
		auto image = camera::captureRGB565(&config::camera::RGB565);

		radioControl->setupFSK(config::radio::fsk);
		radioControl->setupSSTV(config::radio::sstv, sstv);
		radioControl->sendSSTV((uint16_t*)image.buf);

		image::release(image);

		NEXT_SOFT_STATE(sleepState);
	}

	state_t loraFastSSDOState() {
		SSDO ssdoProtocol = SSDO(config::radio::craftIdLoraHDImg, instruments::incGetLoraCounter(), SSDO_TYPE_JPG);

		auto image = camera::captureJpg(&config::camera::jpgHD, JPEG_Q_LOW);

		if (image.buf) {
			radioControl->setupLora(config::radio::loraSSDOFast);

			uint8_t packet[SSDO_PACKET_SIZE];
			for (unsigned retry = 0; retry < 3; retry++)
			for (unsigned i = 0; i < ssdoProtocol.packetsCount(image.len); i++) { 
				unsigned packetLen = ssdoProtocol.setPacket(image.buf, i, packet, image.len);
				radioControl->sendLora(packet, packetLen);
			}

			image::release(image);
			
		}

		NEXT_SOFT_STATE(sleepState);
	}

	state_t loraSlowSSDOState() {
		SSDO ssdoProtocol = SSDO(config::radio::craftIdLoraImg, instruments::incGetLoraCounter(), SSDO_TYPE_JPG);

		auto image = camera::captureJpg(&config::camera::JpgQVGA, JPEG_Q_LOW);

		if (image.buf) {
			radioControl->setupLora(config::radio::loraSSDOFast);

			uint8_t packet[SSDO_PACKET_SIZE];
			for (unsigned retry = 0; retry < 3; retry++)
			for (unsigned i = 0; i < ssdoProtocol.packetsCount(image.len); i++) { 
				unsigned packetLen = ssdoProtocol.setPacket(image.buf, i, packet, image.len);
				radioControl->sendLora(packet, packetLen);
			}

			image::release(image);
			
		}

		NEXT_SOFT_STATE(sleepState);
	}

	state_t loraTelemetryState() {
		String state = "";

		if (config::telemetry::type == TELEMETRY_TYPE_RAWVARS) state = telemetry->getState();
		if (config::telemetry::type == TELEMETRY_TYPE_UKHAS)   state = telemetry->getUKHAS();

    	SSDO ssdoProtocol = SSDO(config::radio::craftIdLoraTelemetry, instruments::incGetLoraCounter(), SSDO_TYPE_TEXT);
    
		radioControl->setupLora(config::radio::loraTelemetry);
		
		uint8_t packet[SSDO_PACKET_SIZE];
    	for (unsigned i = 0; i < ssdoProtocol.packetsCount(state.length()); i++) { 
      		unsigned packetLen = ssdoProtocol.setPacket((uint8_t*)state.c_str(), i, packet, state.length());
    		radioControl->sendLora(packet, packetLen);
    	}

		NEXT_SOFT_STATE(loraSlowSSDOState);
	}

	state_t sleepState() {
		DEBUG_PRINT("Going to deep sleep for ", config::sleep::timeToSleep, "s");
		
		// go to deep sleep
		esp_deep_sleep_start();

		NEXT_HARD_STATE(idleState);
	}
	
}
