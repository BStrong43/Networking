#ifndef A3_PACKET_H
#define A3_PACKET_H

#include <cstring>

// 512 - 1 (typeId) - 1 (queueIndex)
const int PACKET_MAX_BYTES = 510;

enum GameMessageId
{
	INVALID = -1,
	TEST,
	CLIENT_HELLO,
	GAME_START,
	CHAT_REQUEST,
	CHAT_RECIEVE,
	TICTACTOE_MOVE,
	GAME_END
};

struct Packet
{
	unsigned char typeId = INVALID;
	size_t queueIndex = 0;
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