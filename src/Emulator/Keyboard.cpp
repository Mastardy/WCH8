#include "Keyboard.hpp"

#include <Windows.h>

bool Keyboard::mKeys[16] = {};

bool Keyboard::GetKey(uint8_t key)
{
	return mKeys[key];
}

void Keyboard::SetKeys()
{
	mKeys[0x0] = GetAsyncKeyState(0x31) & 0x8000; // 1
	mKeys[0x1] = GetAsyncKeyState(0x32) & 0x8000; // 2
	mKeys[0x2] = GetAsyncKeyState(0x33) & 0x8000; // 3
	mKeys[0x3] = GetAsyncKeyState(0x34) & 0x8000; // 4

	mKeys[0x4] = GetAsyncKeyState(0x51) & 0x8000; // Q
	mKeys[0x5] = GetAsyncKeyState(0x57) & 0x8000; // W
	mKeys[0x6] = GetAsyncKeyState(0x45) & 0x8000; // E
	mKeys[0x7] = GetAsyncKeyState(0x52) & 0x8000; // R

	mKeys[0x8] = GetAsyncKeyState(0x41) & 0x8000; // A
	mKeys[0x9] = GetAsyncKeyState(0x53) & 0x8000; // S
	mKeys[0xA] = GetAsyncKeyState(0x44) & 0x8000; // D
	mKeys[0xB] = GetAsyncKeyState(0x46) & 0x8000; // F

	mKeys[0xC] = GetAsyncKeyState(0x5A) & 0x8000; // Z
	mKeys[0xD] = GetAsyncKeyState(0x58) & 0x8000; // X
	mKeys[0xE] = GetAsyncKeyState(0x43) & 0x8000; // C
	mKeys[0xF] = GetAsyncKeyState(0x56) & 0x8000; // V
}