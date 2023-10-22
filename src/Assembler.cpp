#include "Assembler.hpp"

#include <fstream>
#include <iostream>
#include <format>

namespace Assembler
{
	void Assemble(const std::string& file)
	{
		auto code = ReadFile(file);
		SaveFile(ParseCode(code), file);
	}

	std::vector<std::string> ReadFile(const std::string& file)
	{
		auto code = std::vector<std::string>();

		std::ifstream input(file);

		if (input.fail())
		{
			std::cerr << "Failed to open File: " << file << std::endl;
			exit(2);
		}

		std::string line;
		while (std::getline(input, line))
		{
			code.push_back(line);
		}

		return code;
	}

	std::vector<Byte> ParseCode(const std::vector<std::string>& code)
	{
		auto machineCode = std::vector<Byte>();

		for (const auto& instruction: code)
		{
			if (instruction.substr(0, 3) == "CLS")
			{
				machineCode.emplace_back(0x00);
				machineCode.emplace_back(0xE0);
			}
			else if (instruction.substr(0, 3) == "RET")
			{
				machineCode.emplace_back(0x00);
				machineCode.emplace_back(0xEE);
			}
			else if (instruction.substr(0, 3) == "SYS")
			{
				machineCode.emplace_back(0x0 << 4 | CharToByte(instruction[4]));
				machineCode.emplace_back(CharToByte(instruction[5]) << 4 | CharToByte(instruction[6]));
			}
			else if(instruction.substr(0, 2) == "JP")
			{
				if(instruction[3] == 'V')
				{
					machineCode.emplace_back(0xB << 4 | CharToByte(instruction[7]));
					machineCode.emplace_back(CharToByte(instruction[8]) << 4 | CharToByte(instruction[9]));
				}
				else
				{
					machineCode.emplace_back(0x1 << 4 | CharToByte(instruction[3]));
					machineCode.emplace_back(CharToByte(instruction[4]) << 4 | CharToByte(instruction[5]));
				}
			}
			else if(instruction.substr(0, 4) == "CALL")
			{
				machineCode.emplace_back(0x2 << 4 | CharToByte(instruction[5]));
				machineCode.emplace_back(CharToByte(instruction[6]) << 4 | CharToByte(instruction[7]));
			}
			else if(instruction.substr(0, 2) == "SE")
			{
				if(instruction[7] == 'V')
				{
					machineCode.emplace_back(0x5 << 4 | CharToByte(instruction[4]));
					machineCode.emplace_back(CharToByte(instruction[8]) << 4 | 0x0);
				}
				else
				{
					machineCode.emplace_back(0x3 << 4 | CharToByte(instruction[4]));
					machineCode.emplace_back(CharToByte(instruction[7]) << 4 | CharToByte(instruction[8]));
				}
			}
			else if(instruction.substr(0, 3) == "SNE")
			{
				if(instruction[8] == 'V')
				{
					machineCode.emplace_back(0x9 << 4 | CharToByte(instruction[5]));
					machineCode.emplace_back(CharToByte(instruction[9]) << 4 | 0x0);
				}
				else
				{
					machineCode.emplace_back(0x4 << 4 | CharToByte(instruction[5]));
					machineCode.emplace_back(CharToByte(instruction[8]) << 4 | CharToByte(instruction[9]));
				}
			}
		}

		return machineCode;
	}

	Byte CharToByte(char c)
	{
		switch (c)
		{
			case '0':
				return 0x00;
			case '1':
				return 0x01;
			case '2':
				return 0x02;
			case '3':
				return 0x03;
			case '4':
				return 0x04;
			case '5':
				return 0x05;
			case '6':
				return 0x06;
			case '7':
				return 0x07;
			case '8':
				return 0x08;
			case '9':
				return 0x09;
			case 'A':
				return 0x0A;
			case 'B':
				return 0x0B;
			case 'C':
				return 0x0C;
			case 'D':
				return 0x0D;
			case 'E':
				return 0x0E;
			case 'F':
				return 0x0F;
			default:
				system("cls");
				std::cerr << "Byte not recognized! " << c << '\n';
				std::cin.get();
				exit(2);
				break;
		}

		return 0x0;
	}

	void SaveFile(const std::vector<Byte>& machineCode, const std::string& file)
	{
		std::ofstream output(file.substr(0, file.size() - 3));

		for (const auto& byte : machineCode)
		{
			output.write(reinterpret_cast<const char*>(&byte), sizeof(Byte));
		}

		output.close();
	}
}