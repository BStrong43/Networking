#ifndef __ANIMAL3D_NETWORKINBOUNDEVENT_H
#define __ANIMAL3D_NETWORKINBOUNDEVENT_H

#include "a3_NetworkEvent.h"
#include "RakNet/BitStream.h"

class a3_NetworkInboundEvent : public a3_NetworkEvent
{
protected:
	a3_NetworkInboundEvent(a3_NetGameMessages id, RakNet::BitStream& fromBitStream)
		:a3_NetworkEvent(id)
	{
		fromBitStream.Read(mId);
	}
public:
	virtual void execute() = 0;
};

#endif