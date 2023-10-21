#pragma once

#include <cstdint>

class Keyboard
{
public:
	static bool GetKey(uint8_t key);
	static void SetKeys();

private:
	static bool mKeys[16];
};
