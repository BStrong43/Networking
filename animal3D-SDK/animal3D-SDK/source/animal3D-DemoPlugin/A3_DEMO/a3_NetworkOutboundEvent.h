#ifndef __ANIMAL3D_NETWORKOUTBOUNDEVENT_H
#define __ANIMAL3D_NETWORKOUTBOUNDEVENT_H

#include "a3_NetworkEvent.h"
#include "RakNet/BitStream.h"

class a3_NetworkOutboundEvent : public a3_NetworkEvent
{
public:
	a3_NetworkOutboundEvent(a3_NetGameMessages id)
		:a3_NetworkEvent(id) {}

	virtual void writeToBitStream(RakNet::BitStream& toBitStream)
	{
		toBitStream.Write(mId);
	}
};

#endif