#include "a3KeyboardInput.h"
#include <cstring>

bool a3KeyboardInput::getKeyPress(a3_KeyboardInput* pKeyboard, unsigned char asciiKey)
{
	if (!pKeyboard) return false;
	
	// return true if key was pressed this frame and not last frame
	return pKeyboard->key.key[asciiKey]
		&& pKeyboard->key0.key[asciiKey];
}

void a3KeyboardInput::update(a3_KeyboardInput* pKeyboard)
{
	for(char key = 'A'; key < 'Z'; key++)
	{
		// do not overflow
		if(curTypingIndex < MAX_MESSAGE_LEN)
		{
			// add pressed key to buffer
			if(getKeyPress(pKeyboard, key))
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
