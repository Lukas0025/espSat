#pragma once
#include <Arduino.h>
#define SSDO_PACKET_DATA_SIZE 238 / 2
#define SSDO_PACKET_SIZE 255
#define SSDO_VERSION 1

class SSDO {
	public:
		SSDO(uint32_t senderId, uint32_t objectId);
		uint8_t  setPacket(uint8_t* data, uint32_t packetId, uint8_t* packet, uint32_t dataSize);
    uint32_t packetsCount(uint32_t dataSize) { return ((float) dataSize / SSDO_PACKET_DATA_SIZE) + 1; }
	private:
		uint8_t parity(uint8_t data);
		uint8_t hamming(uint8_t bits);
		uint32_t senderId;
		uint32_t objectId;
};
