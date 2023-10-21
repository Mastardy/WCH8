#pragma once

#include <string>
#include <vector>
#include <cstdint>

#include "GraphicsManager.hpp"

using Byte = uint8_t;
using Word = uint16_t;

class Emulator
{
public:
	Emulator();

	void Run(const std::string& file);

private:
	void ExecuteNextInstruction();

	int ReadFile(const std::string& file);

	void PushStack(Word address);
	Word PopStack();

	void FontSet();

public:
	static constexpr Byte mStackPointerStart = 0x50;
	static constexpr double mFrequency = 1.0 / 500; // 60Hz
	static constexpr double mTimerFrequency = 1.0 / 60; // 60Hz

private:
	GraphicsManager mGraphicsManager;

	Byte mMemory[4096]{}; // 4KB (512B reserved for interpreter)
	Byte mRegisters[16]{}; // Chip-8 has 16 8-bit registers (V0-VF, VF is used as a flag)
	Byte mDelayTimer, mSoundTimer; // 8-bit timer registers that are decremented at a rate of 60Hz
	Word mIndexRegister; // 16-bit register used to store memory addresses (only the lowest 12 bits are used)
	Word mProgramCounter; // 16-bit register that holds the address of the next instruction to be executed
	Byte mStackPointer; // 8-bit register that points to the topmost level of the stack
};