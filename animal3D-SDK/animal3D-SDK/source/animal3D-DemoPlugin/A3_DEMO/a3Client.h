#ifndef A3_CLIENT_H
#define A3_CLIENT_H

#include "a3Peer.h"

class a3Client : public a3Peer
{
public:
	a3Client();

	void init(a3_DemoState* pDemoState) override;
	void cleanup(a3_DemoState* pDemoState) override;

	void input(a3_DemoState* pDemoState) override;
	void networking(a3_DemoState* pDemoState) override;
	void update(a3_DemoState* pDemoState) override;
	void render(a3_DemoState* pDemoState) override;

	void startup(a3_DemoState* pDemoState);

private:
	// testing
	bool isChatting = false;
};

#endif