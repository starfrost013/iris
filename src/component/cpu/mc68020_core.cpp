#include <iostream>
#include <component/cpu/mc68020.hpp>

namespace Iris
{
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
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->addrRegs[7]), "sp [a7]");
        system->AddRegister(new CoherentSystem::Register<uint32_t>(&this->pc), "pc");
        system->AddRegister(new CoherentSystem::Register<uint8_t>(&this->ccr), "ccr"); // flags (technically lower byte of status)
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

        Coherent::SetSystem(system);
        // convert to nanoseconds
        pc = 0x30000000; // temp
        
    }

    uint32_t CPU_MC68020::DecodeEAIndexed(uint16_t seaow)
    {
        uint16_t extension = AddrSpace::ReadU16(pc + 2);

        // on = MC68020 format
        // off = MC68000/68008/68010 format ("brief")
        if (extension & 0x100)
        {
            //old format
        }
        else
        {
            
        }
    }

    /// @brief Decodes the addressing format for most instructins. 68k is very orthogonal.
    /// note: this is not a pointer. this will explode if this emulator wevery becomes multithreaded
    /// source: https://dn721603.ca.archive.org/0/items/bitsavers_motorola68oprocessorUsersManual1984_21835306/MC68020_32-Bit_Microprocessor_Users_Manual_1984.pdf
    uint32_t CPU_MC68020::DecodeEA(uint16_t seaow)
    {
        auto reg = seaow        & 0b00000111; // 0x07
        auto mode = seaow       & 0b00011100; // 0x38
        auto size = seaow       & 0b11000000; // 0xB0

        auto realSize = (size) << 1;
        
        if (realSize == 0) // byte
            realSize = 1;

        uint32_t val;

        switch (reg)
        {
            // These ones are trivial
            case 0b000:
                val = dataRegs[reg];
                break; 
            case 0b001: 
                val = addrRegs[reg];
                break; 
            case 0b010:
                val = AddrSpace::ReadU32(addrRegs[reg]);
                break; 
            case 0b011:
                val = AddrSpace::ReadU32(addrRegs[reg]);
                addrRegs[reg] += realSize;
                break; 
            case 0b100:
                addrRegs[reg] -= realSize;
                val = AddrSpace::ReadU32(addrRegs[reg]);
                break; 
            case 0b101:
                val = AddrSpace::ReadU32((addrRegs[reg] + PCRead16()));
                break; 
            // These ones aren't
            case 0b110:
                val = DecodeEAIndexed(seaow);
                break;

        }
        
    }

    void CPU_MC68020::Tick()
    {
        // read the SEA/OW

        uint16_t next = PCRead16();


        auto iter = instructions.find(next);

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