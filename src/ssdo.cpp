/**
 * espSat project simple esp base satellite
 * File with implementation of SSDO
 * @author Lukas Plevac <lukas@plevac.eu>
 */
#include "ssdo.h"


SSDO::SSDO(uint32_t senderId, uint32_t objectId, uint8_t type) {
	this->senderId   = senderId;
	this->objectId   = objectId;
	this->objectType = type;
}

uint8_t SSDO::setPacket(uint8_t* data, uint32_t packetId, uint8_t* packet, uint32_t dataSize) {
	ssdoHeader_t* packetHeader = (ssdoHeader_t*) packet;
	auto          packetData   = packet + sizeof(ssdoHeader_t);

	uint32_t dataPos = i * SSDO_PACKET_DATA_SIZE;

	//Packet information
	packetHeader->src     = this->senderId;
	packetHeader->pktId   = packetId;
	packetHeader->pktSize = (dataPos + SSDO_PACKET_DATA_SIZE) =< dataSize ? 
								SSDO_PACKET_DATA_SIZE : (dataSize - dataPos); 

	//Object information
	packetHeader->objId   = this->objectId;
	packetHeader->objSize = dataSize;
	packetHeader->objType = this->obejctType;

	//check sum
	packetHeader->crc     = this->calcCRC(&data[dataPos], packetHeader->pktSize);

	for (uint8_t i = 0; i < packetHeader->pktSize; i++) {
		packetData[i] = data[dataPos + i];
	}

	return packetHeader->pktSize + sizeof(ssdoHeader_t);
}

bool SSDO::decodePacket(uint8_t* data, uint8_t* packet, ssdoHeader_t* header) {
	ssdoHeader_t* headerPtr = (ssdoHeader_t*) packet;

	if (memcmp(headerPtr->protName, "SSDO", 4) != 0) return false;
	if (headerPtr->version > SSDO_VERSION)           return false;

	//todo check CRC

	memcpy(header, headerPtr,                     sizeof(ssdoHeader_t));
	memcpy(data,   packet + sizeof(ssdoHeader_t), headerPtr->pktSize);

	return true;
}

uint16_t SSDO::calcCRC(uint8_t data, uint32_t len) {
  uint16_t CRC         = 0xffff; // Seed
  uint16_t xPolynomial = 0x1021;
   
  for (uint32_t i = 0; i < len; i++) {
    CRC ^= message[i] << 8;
    for (unsigned j = 0; j < 8; j++) {
      if (CRC & 0x8000) CRC = (CRC << 1) ^ 0x1021;
      else              CRC <<= 1;
    }
  }

  return CRC;
}