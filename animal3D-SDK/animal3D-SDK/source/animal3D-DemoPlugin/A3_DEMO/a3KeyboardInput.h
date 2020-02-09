#ifndef A3_KEYBOARD_INPUT
#define A3_KEYBOARD_INPUT

#include "animal3D/a3input/a3_KeyboardInput.h"

const size_t MAX_MESSAGE_LEN = 256;

class a3KeyboardInput
{
private:
	bool finishedTyping = false;
	// buffer of characters typed since last reset
	char activeTypingBuffer[MAX_MESSAGE_LEN];
	size_t curTypingIndex = 0;
public:
	a3KeyboardInput() = default;
	// check for a specific key press
	bool getKeyPress(a3_KeyboardInput* pKeyboard, unsigned char asciiKey);
	// adds currently pressed keys A-Z to typing buffer
	void update(a3_KeyboardInput* pKeyboard);
	// clears typing buffer, should be called at beginning of chat
	void resetTypingSession();
	// gets the typing buffer full of characters pressed since last reset
	char* getTypingBuffer();
};

#endif