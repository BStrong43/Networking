#include "a3KeyboardInput.h"
#include <cstring>

a3KeyboardInput::a3KeyboardInput(a3_KeyboardInput* keyboard)
{
	pKeyboard = keyboard;
}

bool a3KeyboardInput::getKetPress(unsigned char asciiKey)
{
	// return true if key was pressed this frame and not last frame
	return pKeyboard->key.key['b']
		&& pKeyboard->key0.key['b'];
}

void a3KeyboardInput::update()
{
	for(char key = 'a'; key < 'z'; key++)
	{
		// do not overflow
		if(curTypingIndex < MAX_MESSAGE_LEN)
		{
			// add pressed key to buffer
			if(getKetPress(key))
			{
				activeTypingBuffer[curTypingIndex++] = key;
			}
		}
	}
}

void a3KeyboardInput::resetTypingSession()
{
	// clear typing buffer to 0s
	memset(activeTypingBuffer, 0, MAX_MESSAGE_LEN);
	// set typing index back to 0
	curTypingIndex = 0;
}

char* a3KeyboardInput::getTypingBuffer()
{
	return activeTypingBuffer;
}
