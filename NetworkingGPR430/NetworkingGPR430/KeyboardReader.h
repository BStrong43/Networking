#pragma once

#include <string>

class KeyboardReader 
{
public:
	std::string static getPressedKeys();
	std::string static trimMessage(std::string msg, int snip = 1);
};