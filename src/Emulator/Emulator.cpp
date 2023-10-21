#include "Emulator.hpp"
#include "Keyboard.hpp"

#include <fstream>
#include <iostream>
#include <chrono>

Emulator::Emulator()
{
	for (unsigned char& i: mMemory)
	{
		i = 0x00;
	}
	for (unsigned char & mRegister : mRegisters)
	{
		mRegister = 0x00;
	}

	FontSet();

	mDelayTimer = 0x0;
	mSoundTimer = 0x0;
	mIndexRegister = 0x00;
	mProgramCounter = 0x200;
	mStackPointer = 0x00;
}

void Emulator::Run(const std::string& file)
{
	auto fileSize = ReadFile(file);

	mGraphicsManager.DrawScreen();

	auto instructionTimer = std::chrono::high_resolution_clock::now();
	auto clockTimer = std::chrono::high_resolution_clock::now();

	while(mProgramCounter < fileSize + 0x200)
	{
		Keyboard::SetKeys();
		auto timeSinceLastInstruction = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - instructionTimer);
		if(timeSinceLastInstruction.count() > mTimerFrequency * 1000)
		{
			ExecuteNextInstruction();
			Keyboard::ResetKeys();
			instructionTimer = std::chrono::high_resolution_clock::now();
		}

		auto timeSinceLastClock = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - clockTimer);
		if(timeSinceLastClock.count() > mFrequency * 1000)
		{
			mGraphicsManager.DrawScreen();
			mSoundTimer = mSoundTimer > 0 ? mSoundTimer - 1 : 0;
			mDelayTimer = mDelayTimer > 0 ? mDelayTimer - 1 : 0;
			clockTimer = std::chrono::high_resolution_clock::now();
		}
	}
}

void Emulator::ExecuteNextInstruction()
{
	std::pair<Byte, Byte> instruction = {mMemory[mProgramCounter], mMemory[mProgramCounter + 1]};

	std::vector<Byte> sprite;

	switch (instruction.first >> 4)
	{
		case 0x0:
			switch (instruction.second)
			{
				case 0xE0:
					mGraphicsManager.ClearScreen();
					mGraphicsManager.DrawScreen();
					break;
				case 0xEE:
					mProgramCounter = PopStack();
					break;
				default:
					// TODO : UNKNOWN BYTES
					break;
			}
			break;
		case 0x1:
			mProgramCounter = (instruction.first & 0x0F) << 8 | instruction.second;
			return;
		case 0x2:
			PushStack(mProgramCounter);
			mProgramCounter = (instruction.first & 0x0F) << 8 | instruction.second;
			return;
		case 0x3:
			if(mRegisters[instruction.first & 0x0F] == instruction.second)
				mProgramCounter += 2;
			break;
		case 0x4:
			if(mRegisters[instruction.first & 0x0F] != instruction.second)
				mProgramCounter += 2;
			break;
		case 0x5:
			if(mRegisters[instruction.first & 0x0F] == mRegisters[instruction.second >> 4])
				mProgramCounter += 2;
			break;
		case 0x6:
			mRegisters[instruction.first & 0x0F] = instruction.second;
			break;
		case 0x7:
			mRegisters[instruction.first & 0x0F] += instruction.second;
			break;
		case 0x8:
			switch (instruction.second & 0xF)
			{
				case 0x0:
					mRegisters[instruction.first & 0x0F] = mRegisters[instruction.second >> 4];
					break;
				case 0x1:
					mRegisters[instruction.first & 0x0F] |= mRegisters[instruction.second >> 4];
					break;
				case 0x2:
					mRegisters[instruction.first & 0x0F] &= mRegisters[instruction.second >> 4];
					break;
				case 0x3:
					mRegisters[instruction.first & 0x0F] ^= mRegisters[instruction.second >> 4];
					break;
				case 0x4:
					mRegisters[instruction.first & 0x0F] += mRegisters[instruction.second >> 4];
					break;
				case 0x5:
					mRegisters[instruction.first & 0x0F] -= mRegisters[instruction.second >> 4];
					break;
				case 0x6:
					if(mRegisters[instruction.first & 0x0F] & 0x01)
						mRegisters[0xF] = 0x01;
					else
						mRegisters[0xF] = 0x00;
					mRegisters[instruction.first & 0x0F] /= 2;
					break;
				case 0x7:
					if(mRegisters[instruction.second >> 4] > mRegisters[instruction.first & 0x0F])
						mRegisters[0xF] = 0x01;
					else
						mRegisters[0xF] = 0x00;
					mRegisters[instruction.first & 0x0F] = mRegisters[instruction.second >> 4] - mRegisters[instruction.first & 0x0F];
					break;
				case 0xE:
					if(mRegisters[instruction.first & 0x0F] & 0x80)
						mRegisters[0xF] = 0x01;
					else
						mRegisters[0xF] = 0x00;
					mRegisters[instruction.first & 0x0F] *= 2;
					break;
				default:
					// TODO : UNKNOWN BYTES
					break;
			}
			break;
		case 0x9:
			if(mRegisters[instruction.first & 0x0F] != mRegisters[instruction.second >> 4])
				mProgramCounter += 2;
			break;
		case 0xA:
			mIndexRegister = (instruction.first & 0x0F) << 8 | instruction.second;
			break;
		case 0xB:
			mProgramCounter = (instruction.first & 0x0F) << 8 | instruction.second + mRegisters[0x0];
			break;
		case 0xC:
			mRegisters[instruction.first & 0x0F] = (rand() % 255) & instruction.second;
			break;
		case 0xD:
			for(int i = 0; i < (instruction.second & 0x0F); ++i)
			{
				sprite.push_back(mMemory[mIndexRegister + i]);
			}
			mRegisters[0xF] = mGraphicsManager.DrawSprite(
					mRegisters[instruction.first & 0x0F],
					mRegisters[instruction.second >> 4],
					sprite
			);
			break;
		case 0xE:
			switch (instruction.second)
			{
				case 0x9E:
					if(Keyboard::GetKey(mRegisters[instruction.first & 0x0F]))
						mProgramCounter += 2;
					break;
				case 0xA1:
					if(!Keyboard::GetKey(mRegisters[instruction.first & 0x0F]))
						mProgramCounter += 2;
					break;
				default:
					// TODO : UNKNOWN BYTES
					break;
			}
			break;
		case 0xF:
			switch (instruction.second)
			{
				case 0x07:
					mRegisters[instruction.first & 0x0F] = mDelayTimer;
					break;
				case 0x0A:
					for(int i = 0; i < 16; ++i)
					{
						if(Keyboard::GetKey(i))
						{
							mRegisters[instruction.first & 0x0F] = i;
							break;
						}
						if(i == 15) i = 0;
					}
					break;
				case 0x15:
					mDelayTimer = mRegisters[instruction.first & 0x0F];
					break;
				case 0x18:
					mSoundTimer = mRegisters[instruction.first & 0x0F];
					break;
				case 0x1E:
					mIndexRegister += mRegisters[instruction.first & 0x0F];
					break;
				case 0x29:
					mIndexRegister = mRegisters[instruction.first & 0x0F] * 5;
					break;
				case 0x33:
					mMemory[mIndexRegister] = mRegisters[instruction.first & 0x0F] / 100;
					mMemory[mIndexRegister + 1] = (mRegisters[instruction.first & 0x0F] / 10) % 10;
					mMemory[mIndexRegister + 2] = mRegisters[instruction.first & 0x0F] % 10;
					break;
				case 0x55:
					for(int i = 0; i <= (instruction.first & 0x0F); ++i)
					{
						mMemory[mIndexRegister + i] = mRegisters[i];
					}
					break;
				case 0x65:
					for(int i = 0; i <= (instruction.first & 0x0F); ++i)
					{
						mRegisters[i] = mMemory[mIndexRegister + i];
					}
					break;
				default:
					// TODO : UNKNOWN BYTES
					break;
			}
			break;
		default:
			// TODO : UNKNOWN BYTES
			break;
	}

	mProgramCounter += 2;
}

void Emulator::PushStack(Word address)
{
	if(mStackPointer == 0x20)
	{
#if defined(_WIN32) || defined(_WIN64)
		system("cls");
#elif defined(__linux__)
		system("clear");
#endif
		std::cerr << "Stack overflow!" << std::endl;
		std::cin.get();
		exit(2);
	}

	mMemory[mStackPointerStart + mStackPointer] = address >> 8;
	mMemory[mStackPointerStart + mStackPointer + 1] = address & 0xFF;
	mStackPointer += 0x02;
}

Word Emulator::PopStack()
{
	if(mStackPointer == 0x00)
	{
		#if defined(_WIN32) || defined(_WIN64)
			system("cls");
		#elif defined(__linux__)
			system("clear");
		#endif

		std::cerr << "Stack underflow!" << std::endl;
		std::cin.get();
		exit(2);
	}

	mStackPointer -= 0x02;
	Word address = mMemory[mStackPointerStart + mStackPointer] << 8 | mMemory[mStackPointerStart + mStackPointer + 1];

	return address;
}

int Emulator::ReadFile(const std::string& file)
{
	std::ifstream input(file, std::ios::binary);

	if (input.fail())
	{
		std::cerr << "Failed to open file: " << file << std::endl;
		std::cin.get();
		exit(127);
	}

	int i = 0x200;
	while (!input.eof())
	{
		Byte byte;
		input.read(reinterpret_cast<char*>(&byte), sizeof(Byte));
		mMemory[i] = byte;
		++i;
	}

	mMemory[i - 1] = 0x00;

	input.close();

	return i - 0x200;
}

void Emulator::FontSet()
{
	// 0
	mMemory[0] = 0xF0;
	mMemory[1] = 0x90;
	mMemory[2] = 0x90;
	mMemory[3] = 0x90;
	mMemory[4] = 0xF0;

	// 1
	mMemory[5] = 0x20;
	mMemory[6] = 0x60;
	mMemory[7] = 0x20;
	mMemory[8] = 0x20;
	mMemory[9] = 0x70;

	// 2
	mMemory[10] = 0xF0;
	mMemory[11] = 0x10;
	mMemory[12] = 0xF0;
	mMemory[13] = 0x80;
	mMemory[14] = 0xF0;

	// 3
	mMemory[15] = 0xF0;
	mMemory[16] = 0x10;
	mMemory[17] = 0xF0;
	mMemory[18] = 0x10;
	mMemory[19] = 0xF0;

	// 4
	mMemory[20] = 0x90;
	mMemory[21] = 0x90;
	mMemory[22] = 0xF0;
	mMemory[23] = 0x10;
	mMemory[24] = 0x10;

	// 5
	mMemory[25] = 0xF0;
	mMemory[26] = 0x80;
	mMemory[27] = 0xF0;
	mMemory[28] = 0x10;
	mMemory[29] = 0xF0;

	// 6
	mMemory[30] = 0xF0;
	mMemory[31] = 0x80;
	mMemory[32] = 0xF0;
	mMemory[33] = 0x90;
	mMemory[34] = 0xF0;

	// 7
	mMemory[35] = 0xF0;
	mMemory[36] = 0x10;
	mMemory[37] = 0x20;
	mMemory[38] = 0x40;
	mMemory[39] = 0x40;

	// 8
	mMemory[40] = 0xF0;
	mMemory[41] = 0x90;
	mMemory[42] = 0xF0;
	mMemory[43] = 0x90;
	mMemory[44] = 0xF0;

	// 9
	mMemory[45] = 0xF0;
	mMemory[46] = 0x90;
	mMemory[47] = 0xF0;
	mMemory[48] = 0x10;
	mMemory[49] = 0xF0;

	// A
	mMemory[50] = 0xF0;
	mMemory[51] = 0x90;
	mMemory[52] = 0xF0;
	mMemory[53] = 0x90;
	mMemory[54] = 0x90;

	// B
	mMemory[55] = 0xE0;
	mMemory[56] = 0x90;
	mMemory[57] = 0xE0;
	mMemory[58] = 0x90;
	mMemory[59] = 0xE0;

	// C
	mMemory[60] = 0xF0;
	mMemory[61] = 0x80;
	mMemory[62] = 0x80;
	mMemory[63] = 0x80;
	mMemory[64] = 0xF0;

	// D
	mMemory[65] = 0xE0;
	mMemory[66] = 0x90;
	mMemory[67] = 0x90;
	mMemory[68] = 0x90;
	mMemory[69] = 0xE0;

	// E
	mMemory[70] = 0xF0;
	mMemory[71] = 0x80;
	mMemory[72] = 0xF0;
	mMemory[73] = 0x80;
	mMemory[74] = 0xF0;

	// F
	mMemory[75] = 0xF0;
	mMemory[76] = 0x80;
	mMemory[77] = 0xF0;
	mMemory[78] = 0x80;
	mMemory[79] = 0x80;
}