#pragma once

#include <cstdint>

class Keyboard
{
public:
	static bool GetKey(uint8_t key);
	static void SetKeys();
	static void ResetKeys();
	static uint8_t WaitKey();

private:
	static uint16_t keys[16];
	static bool mKeys[16];
};
