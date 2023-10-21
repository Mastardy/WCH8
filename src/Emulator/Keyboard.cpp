#include "Keyboard.hpp"

#include <Windows.h>

bool Keyboard::mKeys[16] = {};
uint16_t Keyboard::keys[16] = { 0x31, 0x32, 0x33, 0x34,
								0x51, 0x57, 0x45, 0x52,
								0x41, 0x53, 0x44, 0x46,
								0x5A, 0x58, 0x43, 0x56 };

bool Keyboard::GetKey(uint8_t key)
{
	return mKeys[key];
}

void Keyboard::SetKeys()
{
	for(int i = 0; i < 16; i++)
	{
		if(!mKeys[i]) mKeys[i] = GetAsyncKeyState(keys[i]) & 0x8000;
	}
}

void Keyboard::ResetKeys()
{
	for (int i = 0; i < 16; i++)
		mKeys[i] = false;
}

uint8_t Keyboard::WaitKey()
{
	int currentKey = 0;
	while(true)
	{
		if(GetAsyncKeyState(keys[currentKey]))
		{
			return currentKey;
		}

		++currentKey;
		if(currentKey == 16) currentKey = 0;
	}
}
