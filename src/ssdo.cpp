/**
 * espSat project simple esp base satellite
 * File with implementation of SSDO
 * @author Lukas Plevac <lukas@plevac.eu>
 */
#include "ssdo.h"
#include "debug.h"
#include "radio.h"


SSDO::SSDO(uint32_t senderId, uint32_t objectId, uint8_t type) {
	this->senderId   = senderId;
	this->objectId   = objectId;
	this->objectType = type;
}

uint8_t SSDO::setPacket(uint8_t* data, uint32_t packetId, uint8_t* packet, uint32_t dataSize) {
	auto          packetData   = packet + sizeof(ssdoHeader_t);

	uint32_t dataPos = packetId * SSDO_PACKET_DATA_SIZE;

	ssdoHeader_t newHeader;

	//Packet information
	newHeader.src     = this->senderId;
	newHeader.pktId   = packetId;
	newHeader.pktSize = (dataPos + SSDO_PACKET_DATA_SIZE) <= dataSize ? 
								SSDO_PACKET_DATA_SIZE : (dataSize - dataPos); 

	DEBUG_PRINT(newHeader.pktSize);

	//Object information
	newHeader.objId   = this->objectId;
	newHeader.objSize = dataSize;
	newHeader.objType = this->objectType;

	//check sum
	newHeader.crc     = this->calcCRC(&data[dataPos], newHeader.pktSize);

	memcpy(packet, &newHeader, sizeof(ssdoHeader_t));

	for (uint8_t i = 0; i < newHeader.pktSize; i++) {
		packetData[i] = data[dataPos + i];
	}

	return newHeader.pktSize + sizeof(ssdoHeader_t);
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

bool SSDO::change(RadioControl* radio, LoraSettings_t newSettings, uint8_t resend) {
	ssdoChange_t newSSDOSettings;

	newSSDOSettings.Frequency    = newSettings.Frequency;
	newSSDOSettings.Bandwidth    = newSettings.Bandwidth;
	newSSDOSettings.SpreadFactor = newSettings.SpreadFactor;
	newSSDOSettings.CodeRate     = newSettings.CodeRate;
	newSSDOSettings.SyncWord     = newSettings.SyncWord;

	//chage OBJ type
	uint8_t oldObjType = this->objectType;
	this->objectType   = SSDO_TYPE_CHANGE;

	//create packets and send new config
	uint8_t packet[SSDO_PACKET_SIZE];
	
	for (unsigned retry = 0; retry < resend; retry++)
	for (unsigned i = 0; i < this->packetsCount(sizeof(ssdoChange_t)); i++) { 
		unsigned packetLen = this->setPacket((uint8_t*) &newSSDOSettings, i, packet, sizeof(ssdoChange_t));
		radio->sendLora(packet, packetLen);
	}

	radio->setupLora(newSettings);

	this->objectType = oldObjType;

	return true;
}

uint16_t SSDO::calcCRC(uint8_t* data, uint32_t len) {
  uint16_t CRC         = 0xffff; // Seed
  uint16_t xPolynomial = 0x1021;
   
  for (uint32_t i = 0; i < len; i++) {
    CRC ^= data[i] << 8;
    for (unsigned j = 0; j < 8; j++) {
      if (CRC & 0x8000) CRC = (CRC << 1) ^ 0x1021;
      else              CRC <<= 1;
    }
  }

  return CRC;
}
