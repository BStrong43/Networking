#ifndef A3_PEER_H
#define A3_PEER_H

#include "RakNet/RakPeerInterface.h"

const int MAX_USERNAME_LEN = 16;
const int MAX_NUM_CLIENTS = 10;

struct a3_DemoState;

struct RemoteInfo
{
	char username[MAX_USERNAME_LEN];
};

class a3Peer
{
public:
	virtual ~a3Peer() = default;
	
	virtual void init(a3_DemoState* pDemoState) = 0;
	virtual void cleanup(a3_DemoState* pDemoState) = 0;
	
	virtual void input(a3_DemoState* pDemoState) = 0;
	virtual void networking(a3_DemoState* pDemoState) = 0;
	virtual void update(a3_DemoState* pDemoState) = 0;
	virtual void render(a3_DemoState* pDemoState) = 0;
protected:
	a3Peer();

	RakNet::RakPeerInterface* pRakPeer;
	char username[MAX_USERNAME_LEN];
	RemoteInfo remotes[MAX_NUM_CLIENTS];
	//KeyBoard Info
};

#endif