#include "a3Server.h"
#include "a3NetworkDemoState.h"
#include "animal3D-A3DG/a3graphics/a3_TextRenderer.h"
#include <GL/glew.h>
#include <cstdio>

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
	// C
	if (pDemoState->keyboardInput.getKeyPress(pDemoState->keyboard, 'C'))
	{
		// is not already chatting, start chatting and reset typing buffer
		if (!isChatting)
		{
			isChatting = true;
			pDemoState->keyboardInput.resetTypingSession();
		}
	}
	// Enter
	else if(pDemoState->keyboardInput.getKeyPress(pDemoState->keyboard, a3key_enter))
	{
		// submit chat and stop chatting if chatting
		if(isChatting)
		{
			isChatting = false;
		}
	}

	if (isChatting)
	{
		pDemoState->keyboardInput.update(pDemoState->keyboard);
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

	if(isChatting)
	{
		// draw some text
		a3textDraw(pDemoState->text,
			// center near plane
			0, 0.2f, -1,
			// green
			0, 1, 0, 1,
			"CHATTING");
	}

	char* typingBuffer = pDemoState->keyboardInput.getTypingBuffer();
	// draw some text
	a3textDraw(pDemoState->text,
		// center near plane
		0, 0, -1,
		// green
		0, 1, 0, 1,
		// typing buffer
		"%s",
		typingBuffer);
}
