#include "a3Peer.h"

a3Peer::a3Peer(a3KeyboardInput* keyboardInput)
{
	pKeyboardInput = keyboardInput;
	pRakPeer = RakNet::RakPeerInterface::GetInstance();
}
