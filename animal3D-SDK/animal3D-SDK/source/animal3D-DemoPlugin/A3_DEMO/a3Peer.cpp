#include "a3Peer.h"
#include "a3Client.h"
#include "a3Server.h"
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
		if (true)
			pDemoState->pPeer = new a3Client();
		else
			pDemoState->pPeer = new a3Server();
		pDemoState->activeMode = PLAYER_SELECT;
		break;

	default:
		break;
	}
}

void a3Peer::startup(bool isServer) 
{
	if (isServer)
	{
		//Open connections
	}
	else
	{
		//connect to server
	}
}
