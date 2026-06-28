#include <iostream>
#include <component/cpu/mc68020.hpp>

namespace Iris
{
    void CPU_MC68020::Start()
    {
        Logger::Log("*yawn* I'm a Motorola 68020!");

        // convert to nanoseconds
        pc = 0x30000000; // temp
        
    }

    void CPU_MC68020::Tick()
    {

        // read the SEA/OW

        uint16_t next = PCRead16();
        auto iter = instructions.find(next);

        if (iter != instructions.end())
        {
            Logger::Log(std::format("This is {:x}", iter->second.opcode).c_str(), LogChannels::Debug);

            (this->*iter->second.run)();
            delayNs = (1.0 / (float)clockSpeed) * 1000000000.0 * iter->second.cycles;
        }
    }
}