#include "Disassembler.hpp"

#include <fstream>
#include <iostream>
#include <format>

namespace Disassembler
{
	void Disassemble(const std::string& file)
	{
		auto bytes = ReadFile(file);
		auto operations = ParseOperators(bytes);
		SaveFile(operations, file);
	}

	std::vector<Byte> ReadFile(const std::string& file)
	{
		auto bytes = std::vector<Byte>();

		std::ifstream input(file, std::ios::binary);

		if (input.fail())
		{
			std::cerr << "Failed to open file: " << file << std::endl;
			exit(127);
		}

		while (!input.eof())
		{
			Byte byte;
			input.read(reinterpret_cast<char*>(&byte), sizeof(Byte));
			bytes.push_back(byte);
		}

		bytes.pop_back();

		return bytes;
	}

	std::vector<std::string> ParseOperators(const std::vector<Byte>& bytes)
	{
		auto operations = std::vector<std::string>();

		for (int i = 0; i < bytes.size(); i += 2)
		{
			std::pair<int, int> operation(bytes[i], bytes[i + 1]);

			switch(operation.first >> 4)
			{
				case 0x0:
					if(operation.second == 0xE0) operations.emplace_back("CLS");
					else if(operation.second == 0xEE) operations.emplace_back("RET");
					else operations.emplace_back(std::format("SYS {:01X}{:02X}", operation.first & 0xF, operation.second));
					break;
				case 0x1:
					operations.emplace_back(std::format("JP {:01X}{:02X}", operation.first & 0xF, operation.second));
					break;
				case 0x2:
					operations.emplace_back(std::format("CALL {:01X}{:02X}", operation.first & 0xF, operation.second));
					break;
				case 0x3:
					operations.emplace_back(std::format("SE V{:01X}, {:02X}", operation.first & 0xF, operation.second));
					break;
				case 0x4:
					operations.emplace_back(std::format("SNE V{:01X}, {:02X}", operation.first & 0xF, operation.second));
					break;
				case 0x5:
					operations.emplace_back(std::format("SE V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4));
					break;
				case 0x6:
					operations.emplace_back(std::format("LD V{:01X}, {:02x}", operation.first & 0xF, operation.second));
					break;
				case 0x7:
					operations.emplace_back(std::format("ADD V{:01X}, {:02x}", operation.first & 0xF, operation.second));
					break;
				case 0x8:
					switch(operation.second & 0xF)
					{
						case 0x0:
							operations.emplace_back(std::format("LD V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4));
							break;
						case 0x1:
							operations.emplace_back(std::format("OR V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4));
							break;
						case 0x2:
							operations.emplace_back(std::format("AND V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4));
							break;
						case 0x3:
							operations.emplace_back(std::format("XOR V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4));
							break;
						case 0x4:
							operations.emplace_back(std::format("ADD V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4));
							break;
						case 0x5:
							operations.emplace_back(std::format("SUB V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4));
							break;
						case 0x6:
							operations.emplace_back(std::format("SHR V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4));
							break;
						case 0x7:
							operations.emplace_back(std::format("SUBN V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4));
							break;
						case 0xE:
							operations.emplace_back(std::format("SHL V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4));
							break;
						default:
							operations.emplace_back("UNKNOWN");
							break;
					}
					break;
				case 0x9:
					operations.emplace_back(std::format("SNE V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4));
					break;
				case 0xA:
					operations.emplace_back(std::format("LD I, {:01X}{:02X}", operation.first & 0xF, operation.second));
					break;
				case 0xB:
					operations.emplace_back(std::format("JP V0, {:01X}{:02X}", operation.first & 0xF, operation.second >> 4));
					break;
				case 0xC:
					operations.emplace_back(std::format("RND V{:01X}, {:02X}", operation.first & 0xF, operation.second));
					break;
				case 0xD:
					operations.emplace_back(std::format("DRW V{:01X}, V{:01X}, {:01X}", operation.first & 0xF, operation.second >> 4, operation.second & 0xF));
					break;
				case 0xE:
					switch(operation.second)
					{
						case 0x9E:
							operations.emplace_back(std::format("SKP V{:01X}", operation.first & 0xF));
							break;
						case 0xA1:
							operations.emplace_back(std::format("SKNP V{:01X}", operation.first & 0xF));
							break;
						default:
							operations.emplace_back("UNKNOWN");
							break;
					}
					break;
				case 0xF:
					switch(operation.second)
					{
						case 0x07:
							operations.emplace_back(std::format("LD V{:01X}, DT", operation.first & 0xF));
							break;
						case 0x0A:
							operations.emplace_back(std::format("LD V{:01X}, K", operation.first & 0xF));
							break;
						case 0x15:
							operations.emplace_back(std::format("LD DT, V{:01X}", operation.first & 0xF));
							break;
						case 0x18:
							operations.emplace_back(std::format("LD ST, V{:01X}", operation.first & 0xF));
							break;
						case 0x1E:
							operations.emplace_back(std::format("ADD I, V{:01X}", operation.first & 0xF));
							break;
						case 0x29:
							operations.emplace_back(std::format("LD F, V{:01X}", operation.first & 0xF));
							break;
						case 0x33:
							operations.emplace_back(std::format("LD B, V{:01X}", operation.first & 0xF));
							break;
						case 0x55:
							operations.emplace_back(std::format("LD [I], V{:01X}", operation.first & 0xF));
							break;
						case 0x65:
							operations.emplace_back(std::format("LD V{:01X}, [I]", operation.first & 0xF));
							break;
						default:
							operations.emplace_back("UNKNOWN");
							break;
					}
					break;
				default:
					operations.emplace_back("UNKNOWN");
					break;
			}
		}

		return operations;
	}

	void SaveFile(const std::vector<std::string>& operations, const std::string& file)
	{
		std::ofstream output(file + "asm");

		for (const auto& operation : operations)
		{
			output << operation << std::endl;
		}
		output << '\0';
	}
}