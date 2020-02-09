#include "a3Server.h"
#include "a3NetworkDemoState.h"
#include "animal3D-A3DG/a3graphics/a3_TextRenderer.h"
#include <GL/glew.h>

a3Server::a3Server()
	:a3Peer()
{

}

void a3Server::init(a3_DemoState* pDemoState)
{
}

void a3Server::cleanup(a3_DemoState* pDemoState)
{
}

void a3Server::input(a3_DemoState* pDemoState)
{
	if (pDemoState->keyboardInput.getKetPress('c'))
	{
		// toggle chat
		isChatting = !isChatting;
		// reset typing buffer if starting chat
		if (isChatting)
		{
			pDemoState->keyboardInput.resetTypingSession();
		}
	}

	if (isChatting)
	{
		pDemoState->keyboardInput.update();
	}
}

void a3Server::networking(a3_DemoState* pDemoState)
{
}

void a3Server::update(a3_DemoState* pDemoState)
{

}

void a3Server::render(a3_DemoState* pDemoState)
{
	// clear color
	glClear(GL_COLOR_BUFFER_BIT);

	// draw some text
	a3textDraw(pDemoState->text,
		// center near plane
		0, 0.25f, -1,
		// green
		0, 1, 0, 1,
		// typing buffer
		pDemoState->keyboardInput.getTypingBuffer());
}
