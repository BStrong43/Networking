#include "a3Peer.h"

a3Peer::a3Peer()
{
	pRakPeer = RakNet::RakPeerInterface::GetInstance();
}

void a3Peer::update(a3_DemoState* pDemoState)
{
	switch (pDemoState->activeMode)
	{
	case CONFIG:
		//Decide whether server or client
		//Get username and info to connect/open connections
		break;
	}
}
