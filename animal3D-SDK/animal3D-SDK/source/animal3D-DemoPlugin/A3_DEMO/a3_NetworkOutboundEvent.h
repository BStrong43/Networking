#ifndef __ANIMAL3D_NETWORKOUTBOUNDEVENT_H
#define __ANIMAL3D_NETWORKOUTBOUNDEVENT_H

#include "RakNet/BitStream.h"

class a3_NetworkOutboundEvent
{
protected:
	// TODO: move enum to header so it can be included
	a3_NetGameMessages messageId;

	a3_NetworkOutboundEvent(a3_NetGameMessages id);
public:
	virtual void writeToOutboundEvent(RakNet::BitStream& toBitStream) = 0;
};

#endif