/**
 * espSat project simple esp base satellite
 * File with implementation of state machine of statellite
 * @author Lukas Plevac <lukas@plevac.eu>
 */
#include "stateDrive.h"
#include "persistMem.h"

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  30       /* Time ESP32 will go to sleep (in seconds) */

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
		telemetry    = new Telemetry("ESPCAMSAT-0001", "$$$$$$", "\n");
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
		esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

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
		String state = telemetry->getUKHAS();

		radioControl->setupFSK(config::radio::fsk);
		radioControl->setupRTTY(config::radio::rttySlow, rtty);

		radioControl->sendRTTY(state);

		instruments::incGetTransmitCounter();
		NEXT_SOFT_STATE(sleepState);
	}

	state_t sstvState() {
		uint16_t* image = instruments::cameraCaptureRGB565();

		radioControl->setupFSK(config::radio::fsk);
		radioControl->setupSSTV(config::radio::sstv, sstv);
		radioControl->sendSSTV(image);

		free(image);

		instruments::incGetTransmitCounter();
		NEXT_SOFT_STATE(sleepState);
	}

	state_t loraFastSSDOState() {
		SSDO ssdoProtocol = SSDO(LORA_CRAFT_ID, instruments::incGetLoraCounter(), SSDO_TYPE_JPG);

		auto fb = instruments::cameraCaptureJpgHD();

		radioControl->setupLora(config::radio::loraSSDVFast);

		uint8_t packet[SSDO_PACKET_SIZE];
		for (unsigned i = 0; i < ssdoProtocol.packetsCount(fb->len); i++) { 
			unsigned packetLen = ssdoProtocol.setPacket(fb->buf, i, packet, fb->len);
			radioControl->sendLora(packet, packetLen);
		}

		esp_camera_fb_return(fb);

		instruments::incGetTransmitCounter();
		NEXT_SOFT_STATE(sleepState);
	}

	state_t loraSlowSSDOState() {
		SSDO ssdoProtocol = SSDO(LORA_CRAFT_ID, instruments::incGetLoraCounter(), SSDO_TYPE_JPG);

		auto fb = instruments::cameraCaptureJpgQVGA();

		radioControl->setupLora(config::radio::loraTelemetry);

		uint8_t packet[SSDO_PACKET_SIZE];
		for (unsigned i = 0; i < fb->len; i++) { 
			unsigned packetLen = ssdoProtocol.setPacket(fb->buf, i, packet, fb->len);
			radioControl->sendLora(packet, packetLen);
		}

		esp_camera_fb_return(fb);

		NEXT_SOFT_STATE(sleepState);
	}

	state_t loraTelemetryState() {
		String state = telemetry->getUKHAS();

    	SSDO ssdoProtocol = SSDO(LORA_CRAFT_ID, instruments::incGetLoraCounter(), SSDO_TYPE_TEXT);
    

		radioControl->setupLora(config::radio::loraTelemetry);
		
		uint8_t packet[SSDO_PACKET_SIZE];
    	for (unsigned i = 0; i < ssdoProtocol.packetsCount(state.length()); i++) { 
      		unsigned packetLen = ssdoProtocol.setPacket((uint8_t*)state.c_str(), i, packet, state.length());
    		radioControl->sendLora(packet, packetLen);
    	}

		//powerControl.powerOffRadio();

		instruments::incGetTransmitCounter();

		NEXT_SOFT_STATE(loraSlowSSDOState);
	}

	state_t sleepState() {
		DEBUG_PRINT("Going to deep sleep for ", TIME_TO_SLEEP, "s");
		// go to deep sleep
		esp_deep_sleep_start();

		NEXT_HARD_STATE(idleState);
	}
	
}
