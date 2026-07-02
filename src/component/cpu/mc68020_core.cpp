#include <iostream>
#include <component/cpu/mc68020.hpp>

namespace Iris
{
    #define STRING_REGNAME_SIZE     8
    void CPU_MC68020::Start()
    {
        Logger::Log("*yawn* I'm a Motorola 68020!");

        system = new CoherentSystem();
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->dataRegs[0]), "d0");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->dataRegs[1]), "d1");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->dataRegs[2]), "d2");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->dataRegs[3]), "d3");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->dataRegs[4]), "d4");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->dataRegs[5]), "d5");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->dataRegs[6]), "d6");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->dataRegs[7]), "d7");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->addrRegs[0]), "a0");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->addrRegs[1]), "a1");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->addrRegs[2]), "a2");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->addrRegs[3]), "a3");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->addrRegs[4]), "a4");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->addrRegs[5]), "a5");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->addrRegs[6]), "a6");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->pc), "pc");
        system->AddRegister(new CoherentSystem::Register<uint8_t>(&this->ccr), "ccr"); // flags
        system->AddRegister(new CoherentSystem::Register<uint8_t>(&this->status), "status");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->usp), "usp");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->ssp), "ssp");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->isp), "isp");
        system->AddRegister(new CoherentSystem::Register<uint8_t>(&this->sfc), "sfc");
        system->AddRegister(new CoherentSystem::Register<uint8_t>(&this->dfc), "dfc");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->vbr), "vbr");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->cacr), "cacr");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->caar), "caar");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->cahr), "cahr");

        // convert to nanoseconds
        pc = 0x30000000; // temp
        
    }

    void CPU_MC68020::Tick()
    {
        // read the SEA/OW

        uint16_t next = PCRead16();
        auto iter = instructions.find(next);

        //Logger::Log(std::format("This opcode at 0x{:x} is 0x{:x}", pc, next).c_str(), LogChannels::Debug);

        if (iter != instructions.end())
        {
            (this->*iter->second.run)();
            delayNs = (1.0 / (float)clockSpeed) * 1000000000.0 * iter->second.cycles;
        }
    }

    void CPU_MC68020::Shutdown()
    {
        delete system;
        Coherent::SetSystem(nullptr);
    }
}