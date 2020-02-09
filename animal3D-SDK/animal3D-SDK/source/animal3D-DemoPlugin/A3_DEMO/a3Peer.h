#ifndef A3_PEER_H
#define A3_PEER_H

#include "RakNet/RakPeerInterface.h"
#include "a3Packet.h"

const int MAX_USERNAME_LEN = 256;
const int MAX_NUM_CLIENTS = 10;

struct RemoteInfo
{
	char username[MAX_USERNAME_LEN];
};

class a3Peer
{
public:
	virtual void init();
	virtual void cleanup();
	//input
	//update
	//render
protected:	
	RakNet::RakPeerInterface* pRakPeer;
	char username[MAX_USERNAME_LEN];
	RemoteInfo remotes[MAX_NUM_CLIENTS];
	//KeyBoard Info
};

#endif