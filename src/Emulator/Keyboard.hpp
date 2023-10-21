#pragma once

#include <cstdint>

class Keyboard
{
public:
	static bool GetKey(uint8_t key);
	static void SetKeys();
	static void ResetKeys();

private:
	static uint16_t keys[16];
	static bool mKeys[16];
};
