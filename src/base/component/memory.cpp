#include <base/emulation.hpp>
#include <base/component/memory.hpp>

namespace Iris 
{
    void Memory::Start()
    {
        ram = new uint8_t[Emulation::GetMachine().ramCapacity];
    }

    uint8_t Memory::OnRead8(size_t addr)
    {
        if (addr >= GetRamCapacity())
        {
            Logger::Log("Memory::OnRead8 - Tried to read from invalid RAM address!");
            return 0;
        }   

        return ram[addr];
    }

    uint16_t Memory::OnRead16(size_t addr)
    {
        if (addr >= GetRamCapacity())
        {
            Logger::Log("Memory::OnRead16 - Tried to read from invalid RAM address!");
            return 0;
        } 

        uint16_t* ram16 = (uint16_t*)ram;
        return ram16[addr >> 1];
    }

    uint32_t Memory::OnRead32(size_t addr)
    {
        if (addr >= GetRamCapacity())
        {
            Logger::Log("Memory::OnRead32 - Tried to read from invalid RAM address!");
            return 0;
        } 

        uint32_t* ram32 = (uint32_t*)ram;
        return ram32[addr >> 2];
    }

    void Memory::OnWrite8(size_t addr, uint8_t value)
    {
        if (addr >= GetRamCapacity())
        {
            Logger::Log("Memory::OnWrite8 - Tried to write to invalid RAM address!");
            return;
        } 

        ram[addr] = value;
    }

    void Memory::OnWrite16(size_t addr, uint16_t value)
    {
        if (addr >= GetRamCapacity())
        {
            Logger::Log("Memory::OnWrite16 - Tried to write to invalid RAM address!");
            return;
        } 
    
        uint16_t* ram16 = (uint16_t*)ram;       
        ram16[addr >> 1] = value;
    }

    void Memory::OnWrite32(size_t addr, uint32_t value)
    {
        if (addr >= GetRamCapacity())
        {
            Logger::Log("Memory::OnWrite32 - Tried to write to invalid RAM address!");
            return;
        } 

        uint32_t* ram32 = (uint32_t*)ram;
        ram32[addr >> 2] = value;

    }
}