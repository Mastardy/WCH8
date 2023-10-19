#include "Utility.hpp"
#include <iostream>

void IncorrectUsage(const std::string& exeName)
{
	std::cout << "Incorrect Usage." << std::endl;
	std::cout << "Usage: " << exeName << " <action> <file>" << std::endl;
	std::cout << "Possible Actions: -disasm, -asm, -run" << std::endl;

	exit(127);
}