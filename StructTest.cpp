// StructTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#define PACKET_MAX_BYTES 128

enum GameMessageId
{
	INVALID = -1,
	TEST
};

struct Packet
{
	unsigned char typeId = INVALID;
	char data[PACKET_MAX_BYTES] = {};
	unsigned char queueIndex = 0;
};

void queueData(Packet* packet, char inData[], size_t size)
{
	memcpy(packet->data, inData, size);
	packet->queueIndex += size;
}

void dequeueData(Packet* packet, char outData[], size_t size)
{
	packet->queueIndex -= size;
	memcpy(outData, packet->data, size);
}

int main()
{
    std::cout << "Hello World!\n";

	const size_t intSz = sizeof(int);
	
	Packet* testPacket = new Packet
	{
		TEST
	};

	int x = 10;
	char xBytes[intSz];
	memcpy(xBytes, &x, intSz);
	
	queueData(testPacket, xBytes, intSz);

	int y = 0;
	char yBytes[intSz];

	dequeueData(testPacket, yBytes, intSz);
	memcpy(&y, yBytes, intSz);

	printf("%i\n", y);
}
