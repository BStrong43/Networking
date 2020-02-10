#include "a3Client.h"
#include "a3NetworkDemoState.h"
#include "animal3D-A3DG/a3graphics/a3_TextRenderer.h"
#include <GL/glew.h>
#include <cstdio>

a3Client::a3Client()
	:a3Peer()
{

}

void a3Client::init(a3_DemoState* pDemoState)
{

}

void a3Client::cleanup(a3_DemoState* pDemoState)
{

}

void a3Client::input(a3_DemoState* pDemoState)
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
	else if (pDemoState->keyboardInput.getKeyPress(pDemoState->keyboard, a3key_enter))
	{
		// submit chat and stop chatting if chatting
		if (isChatting)
		{
			isChatting = false;
		}
	}

	if (isChatting)
	{
		pDemoState->keyboardInput.update(pDemoState->keyboard);
	}
}

void a3Client::networking(a3_DemoState* pDemoState)
{

}

void a3Client::update(a3_DemoState* pDemoState)
{
	switch (pDemoState->activeMode)
	{
	case CONFIG:
		a3textdraw(pDemoState->text,
			//near center plane
			0, 0.2f, -1,
			0, 1, 0, 1, "Enter Username: ");
		break;

	case PLAYER_SELECT:

		break;

	case LOBBY:

		break;

	case SPECTATING:

		break;

	case CHALLENGING:

		break;

	case END_GAME:
		
		break;

	default:
		break;
	}
}

void a3Client::render(a3_DemoState* pDemoState)
{
	// clear color
	glClear(GL_COLOR_BUFFER_BIT);

	if (isChatting)
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