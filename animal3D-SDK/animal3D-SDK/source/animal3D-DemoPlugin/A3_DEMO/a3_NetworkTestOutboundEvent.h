#ifndef __ANIMAL3D_NETWORKTESTOUTBOUNDEVENT_H
#define __ANIMAL3D_NETWORKTESTOUTBOUNDEVENT_H

#include "a3_NetworkOutboundEvent.h"
#include "a3_NetworkTestEventData.h"

class a3_NetworkTestOutboundEvent : public a3_NetworkOutboundEvent
{
private:
	a3_NetworkTestEventData mData;
public:
	a3_NetworkTestOutboundEvent(a3_NetworkTestEventData& data);
	void writeToOutboundEvent(RakNet::BitStream& toBitStream) override;
};

#endif