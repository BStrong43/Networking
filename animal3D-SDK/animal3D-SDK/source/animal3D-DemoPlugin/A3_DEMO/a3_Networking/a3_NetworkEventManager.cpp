#include "A3_DEMO/a3_NetworkEventManager.h"

void a3_NetworkEventManager::enqueueInboundEvent(a3_NetworkInboundEvent* inboundEvent)
{
	inboundEventQueue.push(inboundEvent);
}

size_t a3_NetworkEventManager::executeAllInbound()
{
	size_t numDequeued = 0;
	
	a3_NetworkInboundEvent* inboundEvent;
	while((inboundEvent = inboundEventQueue.front()) != nullptr)
	{
		inboundEventQueue.pop();
		inboundEvent->execute();
		delete inboundEvent;
		inboundEvent = nullptr;
		++numDequeued;
	}

	return numDequeued;
}

void a3_NetworkEventManager::enqueueOutboundEvent(a3_NetworkOutboundEvent* outboundEvent)
{
	outboundEventQueue.push(outboundEvent);
}

size_t a3_NetworkEventManager::dequeueAllOutboundToBitStream(RakNet::BitStream& toBitStream)
{
	size_t numDequeued = 0;
	
	a3_NetworkOutboundEvent* outboundEvent;
	while ((outboundEvent = outboundEventQueue.front()) != nullptr)
	{
		inboundEventQueue.pop();
		outboundEvent->writeToBitStream(toBitStream);
		delete outboundEvent;
		outboundEvent = nullptr;
		++numDequeued;
	}

	return numDequeued;
}