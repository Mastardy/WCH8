#include <iostream>
#include "Utility.hpp"
#include "Disassembler.hpp"
#include "Assembler.hpp"
#include "Emulator/Emulator.hpp"

int main(int argc, char const* argv[])
{
	if (argc != 3) IncorrectUsage(argv[0]);

	std::string action = argv[1];
	std::string file = argv[2];

	if (action == "-disasm") Disassembler::Disassemble(file);
	else if (action == "-asm") Assembler::Assemble(file);
	else if (action == "-run")
	{
		Emulator emulator;
		emulator.Run(file);
		std::cin.get();
	} else IncorrectUsage(argv[0]);

	return 0;
}
