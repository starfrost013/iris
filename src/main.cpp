/*
IRIS - A 68000 SGI Emulator!
Copyright (C) 2026 starfrost
*/

#include <iostream>

namespace Iris
{
    int main(int argc, char** argv)
    {
        std::cout << "Hi! I'm an SGI Iris 3130!" << std::endl;
    }
}

int main(int argc, char** argv)
{
    Iris::main(argc, argv);
}