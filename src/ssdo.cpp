#include "ssdo.h"


SSDO::SSDO(uint32_t senderId, uint32_t objectId) {
	this->senderId = senderId;
	this->objectId = objectId;
}

uint8_t SSDO::setPacket(uint8_t* data, uint32_t packetId, uint8_t* packet, uint32_t dataSize) {
	uint32_t* packet32    = (uint32_t*) packet;
	uint32_t* packet32AFV = (uint32_t*)(packet + 5);	

	//shift data
	data            += packetId * SSDO_PACKET_DATA_SIZE;
	uint32_t dataPos = packetId * SSDO_PACKET_DATA_SIZE;


	/*
	 * Set header
	 *   0   1   2   3        4          5 6 7 8    9 10 11 12  13 14 15 16  17 ... 254
	 * | S | S | D | O | SSDO_VERSION | SENDER_ID | PACKET_ID | OBJECT_ID  | DATA
	 */
	packet32   [0] = 0x5353444F; // SSDO
	packet     [4] = SSDO_VERSION;
	packet32AFV[0] = this->senderId;
	packet32AFV[1] = packetId;
	packet32AFV[2] = this->objectId;


	//shift packet pointer after header
	packet += 17;

	/*
	 * Calculate data with hamming
         */
	for (unsigned i = 0; i < SSDO_PACKET_DATA_SIZE; i++) {
		if (dataPos + i >= dataSize) return i + 17;

		packet[i * 2    ] = this->hamming(packet[i]);
		packet[i * 2 + 1] = this->hamming(packet[i] >> 4);		
	}

	return 255;
}

uint8_t SSDO::parity(uint8_t data) {
    uint8_t parity = data;

    for (unsigned i = 0; i < 8; i += 1) {
        parity ^= (data >> i); 
    }

    return data & 1;
}

uint8_t SSDO::hamming(uint8_t bits) {
	uint8_t p3 = this->parity(                 (bits >> 1) & 0b111);
	uint8_t p1 = this->parity((bits & 0b1 ) | ((bits >> 1) & 0b110));
	uint8_t p0 = this->parity((bits & 0b11) | ((bits >> 1) & 0b100));
	uint8_t p7 = this->parity( bits & 0xF );

	uint8_t res = p0 | (p1 << 1) | ((bits & 0b1) << 2) | (p3 << 3) | ((bits & 0b1110) << 3) | (p7 << 7);
}
