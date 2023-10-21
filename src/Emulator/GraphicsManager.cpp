#include "GraphicsManager.hpp"

#include <Windows.h>
#include <iostream>

GraphicsManager::GraphicsManager()
{
	mScreen = {};
}

void GraphicsManager::ClearScreen()
{
	mScreen = {};
}

void GraphicsManager::DrawScreen()
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
	std::string screen;

	for(int i = -1; i < screenWidth + 1; ++i)
		screen += '-';
	screen += '\n';

	for(int i = 0; i < screenHeight; ++i)
	{
		screen += '|';
		for(int j = 0; j < screenWidth; ++j)
		{
			screen += mScreen[i][j] ? (char)254u : ' ';
		}
		screen += '|';
		screen += '\n';
	}

	for(int i = -1; i < screenWidth + 1; ++i)
		screen += '-';

	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), screen.c_str(), (screenWidth + 3) * (screenHeight + 2), nullptr, nullptr);
}

bool GraphicsManager::DrawSprite(Byte x, Byte y, std::vector<Byte> sprite)
{
	bool collision = false;

	for(int i = 0; i < sprite.size(); i++)
	{
		for(int j = 0; j < 8; j++)
		{
			if (!(sprite[i] & (0x80 >> j))) continue;
			if(mScreen[(y + i) % screenHeight][(x + j) % screenWidth]) collision = true;
			mScreen[(y + i) % screenHeight][(x + j) % screenWidth] ^= 1;
		}
	}
	return collision;
}