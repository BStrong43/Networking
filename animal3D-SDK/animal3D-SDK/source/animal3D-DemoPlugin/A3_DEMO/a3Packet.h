#ifndef A3_PACKET_H
#define A3_PACKET_H

#include <cstring>

// 256 - 1 (typeId) - 1 (queueIndex)
const int PACKET_MAX_BYTES = 254;

enum GameMessageId
{
	INVALID = -1,
	TEST
};

struct Packet
{
	unsigned char typeId = INVALID;
	unsigned char queueIndex = 0;
	char data[PACKET_MAX_BYTES] = {};
};

inline void packetQueueData(Packet* packet, char inData[], size_t size)
{
	memcpy(packet->data, inData, size);
	packet->queueIndex += size;
}

inline void packetDequeueData(Packet* packet, char outData[], size_t size)
{
	packet->queueIndex -= size;
	memcpy(outData, packet->data, size);
}

#endif