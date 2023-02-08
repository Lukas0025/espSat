#pragma once

#include "./radio.h"
#include "./config.h"
#include "./telemetry.h"
#include "./instruments.h"
#include "./testData.h"
#include "./ssdo.h"

#include <Arduino.h>

#define NEXT_SOFT_STATE(X) {currentState = X; return; }
#define NEXT_HARD_STATE(X) {currentState = X; return; }

#define INIT_ALT 100
#define LORA_CRAFT_ID 0x1

namespace StateDrive {
	typedef void state_t;
	void run();
	void setup(RADIOHW* radioSX);
	state_t initState();
	state_t idleState();
	state_t rttyState();
	state_t sstvState();
	state_t loraFastSSDOState();
	state_t loraSlowSSDOState();
	state_t loraTelemetryState();
	state_t sleepState();	
}
