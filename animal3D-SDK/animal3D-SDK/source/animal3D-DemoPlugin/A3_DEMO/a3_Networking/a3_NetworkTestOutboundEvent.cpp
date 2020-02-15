#include "A3_DEMO/a3_NetworkTestOutboundEvent.h"
#include "A3_DEMO/a3_NetworkTestEventData.h"

a3_NetworkTestOutboundEvent::a3_NetworkTestOutboundEvent(a3_NetworkTestEventData& data)
	:a3_NetworkOutboundEvent(ID_GAME_MESSAGE_1)
{
	mData = data;
}

void a3_NetworkTestOutboundEvent::writeToOutboundEvent(RakNet::BitStream& toBitStream)
{
	toBitStream.Write(mData.test);
}
