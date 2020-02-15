#ifndef __ANIMAL3D_NETWORKTESTINBOUNDEVENT_H
#define __ANIMAL3D_NETWORKTESTINBOUNDEVENT_H

#include "a3_NetworkInboundEvent.h"
#include "a3_NetworkTestEventData.h"
#include "RakNet/BitStream.h"

class a3_NetworkTestInboundEvent : public a3_NetworkInboundEvent
{
private:
	a3_NetworkTestEventData mData;
public:
	a3_NetworkTestInboundEvent(RakNet::BitStream& fromBitStream);
	void execute() override;
};

#endif