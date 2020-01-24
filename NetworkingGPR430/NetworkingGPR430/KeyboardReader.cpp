#include "KeyboardReader.h"
#include <Windows.h>
#include <WinUser.h>
std::string KeyboardReader::getPressedKeys()
{
	std::string keysPressed = "";
	
	//Reading specific bytes from short -> https://stackoverflow.com/questions/7895888/getasynckeystates-return-values-dont-seem-to-correspond-with-the-description
	//Specific Keys
	if (!!(GetAsyncKeyState(VK_BACK) & 0x8000)) //Backspace
	{
		return "-1";
	}
	if (!!(GetAsyncKeyState(VK_RETURN) & 0x8000)) //Enter
	{
		return "-2";
	}
	
	// \/Thanks to Dan\/
	for (char i = 0x41; i <= 0x5A; ++i)
	{
		if (!!(GetAsyncKeyState(i) & 0x8000))// a-z
		{
			if (!!(GetAsyncKeyState(VK_SHIFT) & 0x8000)) //Shift
				keysPressed += i;
			else
				keysPressed += tolower(i);
		}
	}

	return keysPressed;
}

std::string KeyboardReader::trimMessage(std::string msg, int snip)
{
	for (int i = 0; i < snip; i++)
	{
		msg.pop_back();
		msg.shrink_to_fit();
	}
	return msg;
}