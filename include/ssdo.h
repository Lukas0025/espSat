/**
 * espSat project simple esp base satellite
 * File with header of slow scan digital object protocol
 * @author Lukas Plevac <lukas@plevac.eu>
 */

#pragma once
#include <Arduino.h>

#define SSDO_PACKET_DATA_SIZE 238 / 2
#define SSDO_PACKET_SIZE 255
#define SSDO_VERSION 1

class SSDO {
	public:
		/**
		 * Constructor of SSDO (Slow Scan Digital Object)
		 * Data encoder/decoder for unbearable medium transfer
		 * @param senderId id of trasmitter set do not use if only recive
		 * @param objectId id of trasferd object do not use if only recive
		 */
		SSDO(uint32_t senderId, uint32_t objectId);

		/**
		 * Create new SSDO packet
		 * @param data poiter to data to send
		 * @param packetId id of packet in data (fisrt is 0)
		 * @param packet   pointer to packet
		 * @param dataSize Size of data to send
		 */
		uint8_t  setPacket(uint8_t* data, uint32_t packetId, uint8_t* packet, uint32_t dataSize);
    	
		/**
		 * Get count of SSDO packets for data size
		 * @param dataSize size of data to packetize
		 */
		uint32_t packetsCount(uint32_t dataSize) { return ((float) dataSize / SSDO_PACKET_DATA_SIZE) + 1; }
	private:
		uint8_t parity(uint8_t data);
		uint8_t hamming(uint8_t bits);
		uint32_t senderId;
		uint32_t objectId;
};
