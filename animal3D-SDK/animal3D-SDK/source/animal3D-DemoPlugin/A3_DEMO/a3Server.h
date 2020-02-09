#ifndef A3_SERVER_H
#define A3_SERVER_H

#include "a3Peer.h"

class a3Server : public a3Peer
{
public:
	a3Server();

	void init(a3_DemoState* pDemoState) override;
	void cleanup(a3_DemoState* pDemoState) override;
	
	void input(a3_DemoState* pDemoState) override;
	void networking(a3_DemoState* pDemoState) override;
	void update(a3_DemoState* pDemoState) override;
	void render(a3_DemoState* pDemoState) override;

private:
	// testing
	bool isChatting = false;
};

#endif