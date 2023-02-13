/**
 * espSat project simple esp base satellite
 * File with header of state machine (state drive of satellite)
 * @author Lukas Plevac <lukas@plevac.eu>
 */
#pragma once

#include "radio.h"
#include "config.h"
#include "telemetry.h"
#include "instruments.h"
#include "testData.h"
#include "ssdo.h"
#include "persistMem.h"

#include <Arduino.h>

#define NEXT_SOFT_STATE(X) {currentState = X; return; }
#define NEXT_HARD_STATE(X) {currentState = X; PersistMem::setLastState(X); return; }

#define INIT_ALT 100
#define LORA_CRAFT_ID 0x1

namespace StateDrive {
	typedef void state_t;
	
	/**
	 * run current state
	 */
	void run();
	
	/**
	 * Setup state machine
	 * @param radioSX radio to work with
	 */
	void setup(RADIOHW* radioSX);

	/**
	 * init state wait for activation in altudite X
	 * if alt() > X  NEXT_STATE(idleState) 
	 * else          NEXT_STATE(initState)
	 */
	state_t initState();

	/**
	 * idle state - diagnotize satellite select next state by state of stallite
	 * if tc % 6 == 0 NEXT_STATE(loraTelemetryState) 
	 * if tc % 6 == 1 NEXT_STATE(rttyState)
	 * if tc % 6 == 2 NEXT_STATE(loraTelemetryState) 
	 * if tc % 6 == 3 NEXT_STATE(sstvState)
	 * if tc % 6 == 4 NEXT_STATE(loraTelemetryState) 
	 * if tc % 6 == 5 NEXT_STATE(loraFastSSDOState)
	 */
	state_t idleState();

	/**
	 * rtty state - send radio RTTY telemetry message
	 * NEXT_STATE(sleepState)
	 */
	state_t rttyState();
	
	/**
	 * sstv state - send radio SSTV camera image
	 * NEXT_STATE(sleepState)
	 */
	state_t sstvState();

	/**
	 * loraFastSSDO State - send radio HD lora SSDO camera image
	 * NEXT_STATE(sleepState)
	 */
	state_t loraFastSSDOState();

	/**
	 * loraSlowSSDO State - send radio Low-Res lora SSDO camera image
	 * NEXT_STATE(sleepState)
	 */
	state_t loraSlowSSDOState();

	/**
	 * loraTelemetry State - send radio lora telemetry message
	 * NEXT_STATE(loraSlowSSDOState)
	 */
	state_t loraTelemetryState();

	/*
	 * sleep State - go to deep sleep
 	 * NEXT_STATE(idleState)
	 * @pre set all importat varaibles to presistent memory else it will be erased
	 */
	state_t sleepState();	
}
