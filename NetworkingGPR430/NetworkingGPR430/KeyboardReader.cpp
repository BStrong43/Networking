#include "KeyboardReader.h"
#include <Windows.h>
#include <WinUser.h>
std::string KeyboardReader::getPressedKeys()
{
	std::string keysPressed = "";

	//Specific Keys
	if (GetAsyncKeyState(8)) //Backspace
	{
		return "-1";
	}
	else if (GetAsyncKeyState(13)) //Enter
	{
		return "0";
	}

	for (char i = 'a'; i <= 'z'; ++i)
	{
		if (GetAsyncKeyState(i))// a-z
		{
			if (GetAsyncKeyState(16)) //Shift
				keysPressed += toupper(i);
			else
				keysPressed += i;
		}
	}

	return keysPressed;
}

std::string KeyboardReader::trimMessage(std::string msg, int snip = 1)
{
	for (int i = 0; i < snip; i++)
	{
		msg = msg.pop_back;
	}
	return msg;
}