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
	}

	std::vector<Byte> ReadFile(const std::string& file)
	{
		auto bytes = std::vector<Byte>();

		std::ifstream input(file, std::ios::binary);

		if (input.fail())
		{
			std::cout << "Failed to open file: " << file << std::endl;
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
			auto nibble = operation.first >> 4;

			std::cout << std::format("{:X} {:02X} {:02X}   ", i + 0x200, operation.first, operation.second);

			int subNibble, x, y;

			switch(nibble)
			{
				case 0x0:
					if(operation.second == 0xE0) std::cout << "CLS";
					else if(operation.second == 0xEE) std::cout << "RET";
					else std::cout << std::format("SYS {:01X}{:02X}", operation.first & 0xF, operation.second);
					break;
				case 0x1:
					std::cout << std::format("JP {:01X}{:02X}", operation.first & 0xF, operation.second);
					break;
				case 0x2:
					std::cout << std::format("CALL {:01X}{:02X}", operation.first & 0xF, operation.second);
					break;
				case 0x3:
					std::cout << std::format("SE V{:01X}, {:02X}", operation.first & 0xF, operation.second);
					break;
				case 0x4:
					std::cout << std::format("SNE V{:01X}, {:02X}", operation.first & 0xF, operation.second);
					break;
				case 0x5:
					std::cout << std::format("SE V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4);
					break;
				case 0x6:
					std::cout << std::format("LD V{:01X} {:02x}", operation.first & 0xF, operation.second);
					break;
				case 0x7:
					std::cout << std::format("ADD V{:01X} {:02x}", operation.first & 0xF, operation.second);
					break;
				case 0x8:
					switch(operation.second & 0xF)
					{
						case 0x0:
							std::cout << std::format("LD V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4);
							break;
						case 0x1:
							std::cout << std::format("OR V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4);
							break;
						case 0x2:
							std::cout << std::format("AND V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4);
							break;
						case 0x3:
							std::cout << std::format("XOR V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4);
							break;
						case 0x4:
							std::cout << std::format("ADD V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4);
							break;
						case 0x5:
							std::cout << std::format("SUB V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4);
							break;
						case 0x6:
							std::cout << std::format("SHR V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4);
							break;
						case 0x7:
							std::cout << std::format("SUBN V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4);
							break;
						case 0xE:
							std::cout << std::format("SHL V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4);
							break;
						default:
							std::cout << "UNKN";
							break;
					}
					break;
				case 0x9:
					std::cout << std::format("SNE V{:01X}, V{:01X}", operation.first & 0xF, operation.second >> 4);
					break;
				case 0xA:
					std::cout << std::format("LD I, {:01X}{:02X}", operation.first & 0xF, operation.second);
					break;
				case 0xB:
					std::cout << std::format("JP V0, {:01X}{:02X}", operation.first & 0xF, operation.second >> 4);
					break;
				case 0xC:
					std::cout << std::format("RND V{:01X}, {:02X}", operation.first & 0xF, operation.second);
					break;
				case 0xD:
					std::cout << std::format("DRW V{:01X}, V{:01X}, {:01X}", operation.first & 0xF, operation.second >> 4, operation.second & 0xF);
					break;
				case 0xE:
					switch(operation.second)
					{
						case 0x9E:
							std::cout << std::format("SKP V{:01X}", operation.first & 0xF);
							break;
						case 0xA1:
							std::cout << std::format("SKNP V{:01X}", operation.first & 0xF);
							break;
						default:
							std::cout << "UNKN";
							break;
					}
					break;
				case 0xF:
					switch(operation.second)
					{
						case 0x07:
							std::cout << std::format("LD V{:01X}, DT", operation.first & 0xF);
							break;
						case 0x0A:
							std::cout << std::format("LD V{:01X}, K", operation.first & 0xF);
							break;
						case 0x15:
							std::cout << std::format("LD DT, V{:01X}", operation.first & 0xF);
							break;
						case 0x18:
							std::cout << std::format("LD ST, V{:01X}", operation.first & 0xF);
							break;
						case 0x1E:
							std::cout << std::format("ADD I, V{:01X}", operation.first & 0xF);
							break;
						case 0x29:
							std::cout << std::format("LD F, V{:01X}", operation.first & 0xF);
							break;
						case 0x33:
							std::cout << std::format("LD B, V{:01X}", operation.first & 0xF);
							break;
						case 0x55:
							std::cout << std::format("LD [I], V{:01X}", operation.first & 0xF);
							break;
						case 0x65:
							std::cout << std::format("LD V{:01X}, [I]", operation.first & 0xF);
							break;
						default:
							std::cout << "UNKN";
							break;
					}
					break;
				default:
					std::cout << "UNKN";
					break;
			}

			std::cout << std::endl;
		}

		return operations;
	}
}