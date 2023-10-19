#include "Disassembler.hpp"

#include <fstream>
#include <iostream>

void Disassembler::Disassemble(const std::string &file)
{
	auto bytes = ReadFile(file);

	for(auto byte : bytes)
	{
		std::cout << std::hex << static_cast<int>(byte) << std::endl;
	}
}

std::vector<Byte> Disassembler::ReadFile(const std::string &file)
{
	auto bytes = std::vector<Byte>();

	std::ifstream input(file, std::ios::binary);

	if(input.fail())
	{
		std::cout << "Failed to open file: " << file << std::endl;
		exit(127);
	}

	while(!input.eof())
	{
		Byte byte;
		input.read(reinterpret_cast<char*>(&byte), sizeof(Byte));
		bytes.push_back(byte);
	}

	return bytes;
}