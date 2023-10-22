#pragma once

#include <string>
#include <vector>
#include <cstdint>

using Byte = uint8_t;
using Word = uint16_t;

namespace Assembler
{
	void Assemble(const std::string& file);

	std::vector<std::string> ReadFile(const std::string& file);
	std::vector<Byte> ParseCode(const std::vector<std::string>& code);
	void SaveFile(const std::vector<Byte>& machineCode, const std::string& file);
	Byte CharToByte(char c);
};
