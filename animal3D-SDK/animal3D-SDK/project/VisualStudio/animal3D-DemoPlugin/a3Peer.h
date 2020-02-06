#pragma once
#include "RakNet/RakPeerInterface.h"

const int MAX_USERNAME_LEN = 256;
const int MAX_NUM_CLIENTS = 10;

struct RemoteInfo
{
	char username[MAX_USERNAME_LEN];
};

class a3Peer
{
public:
	//input
	//update
	//render
protected:
	RakNet::RakPeerInterface* a3Peer;
	char username[MAX_USERNAME_LEN];
	RemoteInfo remotes[MAX_NUM_CLIENTS];
	//KeyBoard Info
};