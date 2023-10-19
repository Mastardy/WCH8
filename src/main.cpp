#include <iostream>

void IncorrectUsage(const std::string& exeName)
{
    std::cout << "Incorrect Usage." << std::endl;
    std::cout << "Usage: " << exeName << " <action> <file>" << std::endl;
    std::cout << "Possible Actions: -disasm, -asm, -run" << std::endl;

    exit(127);
}

int main(int argc, char const *argv[]) {
    if(argc != 3) IncorrectUsage(argv[0]);

    std::string action = argv[1];
    std::string file = argv[2];

    return 0;
}
