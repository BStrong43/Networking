#ifndef __ANIMAL3D_NETWORKEVENTMANAGER_H
#define __ANIMAL3D_NETWORKEVENTMANAGER_H

#include <queue>
#include "a3_NetworkInboundEvent.h"
#include "a3_NetworkOutboundEvent.h"

class a3_NetworkEventManager
{
private:
	std::queue<a3_NetworkInboundEvent*> inboundEventQueue;
	std::queue<a3_NetworkOutboundEvent*> outboundEventQueue;
public:
	void enqueueInboundEvent(a3_NetworkInboundEvent* inboundEvent);
	size_t executeAllInbound();

	void enqueueOutboundEvent(a3_NetworkOutboundEvent* outboundEvent);
	size_t dequeueAllOutboundToBitStream(RakNet::BitStream& toBitStream);
};

#endif