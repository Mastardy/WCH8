#pragma once

#include <string>
#include <vector>
#include <cstdint>

using Byte = uint8_t;

namespace Disassembler
{
	void Disassemble(const std::string& file);

	std::vector<Byte> ReadFile(const std::string& file);
	std::vector<std::string> ParseOperators(const std::vector<Byte>& bytes);
	void SaveFile(const std::vector<std::string>& operations, const std::string& file);
};