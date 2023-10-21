#pragma once

#include <cstdint>
#include <array>
#include <vector>

using Byte = uint8_t;
using Word = uint16_t;

class GraphicsManager
{
public:
	GraphicsManager();

	void DrawScreen();
	bool DrawSprite(Byte x, Byte y, std::vector<Byte> sprite);
	void ClearScreen();

private:
	static constexpr unsigned char screenWidth = 64;
	static constexpr unsigned char screenHeight = 32;
	std::array<std::array<bool, screenWidth>, screenHeight> mScreen{};
};