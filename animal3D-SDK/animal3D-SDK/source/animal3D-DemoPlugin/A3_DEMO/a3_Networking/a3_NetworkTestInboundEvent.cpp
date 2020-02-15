#include "A3_DEMO/a3_NetworkTestInboundEvent.h"

a3_NetworkTestInboundEvent::a3_NetworkTestInboundEvent(RakNet::BitStream& fromBitStream)
	:a3_NetworkInboundEvent(ID_GAME_MESSAGE_1, fromBitStream)
{
	fromBitStream.Read(mData.test);
}

void a3_NetworkTestInboundEvent::execute()
{
	printf("%i", mData.test);
}
