/**
 * espSat project simple esp base satellite
 * File with header of slow scan digital object protocol
 * @author Lukas Plevac <lukas@plevac.eu>
 */

#pragma once
#include <Arduino.h>

#define SSDO_PACKET_DATA_SIZE (SSDO_PACKET_SIZE - sizeof(ssdoHeader_t))
#define SSDO_PACKET_SIZE 255
#define SSDO_VERSION 1

#define SSDO_TYPE_TEXT 0
#define SSDO_TYPE_JPG  1

typedef struct {
	char     protName[4] = {'S', 'S', 'D', 'O'};
	uint8_t  version     = SSDO_VERSION;
	uint32_t src         = 0;
	uint32_t pktId       = 0;
	uint8_t  pktSize     = 0;
	uint32_t objId       = 0;
	uint32_t objSize     = 0;
	uint8_t  objType     = 0;
	uint16_t crc         = 0;
} __attribute__((packed)) ssdoHeader_t;


class SSDO {
	public:
		/**
		 * Constructor of SSDO (Slow Scan Digital Object)
		 * Data encoder/decoder for unbearable medium transfer
		 * @param senderId id of trasmitter set do not use if only recive
		 * @param objectId id of trasferd object do not use if only recive
		 * @param type type id of object 
		 */
		SSDO(uint32_t senderId, uint32_t objectId, uint8_t type);

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

		/**
		 * Decode SSDO packet 
		 * @param data    pointer to save decoded data from packet
		 * @param packet  pointer to packet to decode
		 * @param header  pointer to save decoded header
		 * @return bool true if success decode
		 */
		bool decodePacket(uint8_t* data, uint8_t* packet, ssdoHeader_t* header);

	private:
		/**
		 * Calculate CRC check sum for data
		 * @param data pointer to data to calc CRC
		 * @param len  length of data
		 * @return CRC in uint16_t
		 */
		uint16_t calcCRC(uint8_t *data, uint32_t len);
		
		uint32_t senderId;
		uint32_t objectId;
		uint8_t  objectType;
};
