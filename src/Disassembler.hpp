#pragma once

#include <string>
#include <vector>
#include <cstdint>

using Byte = uint8_t;

class Disassembler
{
public:
	static void Disassemble(const std::string& file);

private:
	static std::vector<Byte> ReadFile(const std::string& file);
};