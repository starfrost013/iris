#include <iostream>
#include <cpu/mc68020.hpp>

namespace Iris
{
    void CPU_MC68020::Start()
    {
        std::cout << "*yawn* I'm a Motorola 68020!" << std::endl;
    }

    void CPU_MC68020::Execute(uint16_t opcode)
    {

    }

    void CPU_MC68020::Tick()
    {
        
    }
}