#pragma once 
#include <Iris.hpp>
#include <base/filesystem/filesystem.hpp>
#include <component/addrspace.hpp>

namespace Iris
{
    extern Cvar* promPath;
    extern Cvar* promSize;

    #define SRAM_START          0x33000000
    #define SRAM_SIZE           2048

    // FOR COMPONENTS, WE DON'T NEED TO BOUNDS CHECK BECAUSE WE ALREADY MAPPED IT!

    class PROM_SRAM : public Component
    {
    public: 
        void Start()
        {
            // map the private ram
            AddrSpaceMapping mapping = AddrSpaceMapping();

            mapping.startAddr = SRAM_START;
            mapping.endAddr = mapping.startAddr + SRAM_SIZE;
            mapping.component = this;
            AddrSpace::AddMapping(mapping);
        }

        const char* GetName() { return "IRIS 3130 System PROM Private SRAM [MCM2016HN16 - IP2/U95]"; };

        uint8_t OnRead8(size_t addr) override 
        { 
            addr %= (size_t)SRAM_SIZE;
            return (sram[addr]); 
        };

        uint16_t OnRead16(size_t addr) override 
        { 
            addr %= (size_t)SRAM_SIZE;
            uint16_t* rom16 = (uint16_t*)sram; 
            return rom16[addr >> 1]; 
        };

        uint32_t OnRead32(size_t addr) override 
        { 
            addr %= (size_t)SRAM_SIZE;
            uint32_t* rom32 = (uint32_t*)sram; 
            return rom32[addr >> 2]; 
        };

        void OnWrite8(size_t addr, uint8_t value) 
        { 
            addr %= (size_t)SRAM_SIZE;
            sram[addr] = value; 
        };

        void OnWrite16(size_t addr, uint16_t value) 
        { 
            addr %= (size_t)SRAM_SIZE;
            uint16_t* rom16 = (uint16_t*)sram; 
            rom16[addr >> 1] = value; 
        };

        void OnWrite32(size_t addr, uint32_t value)
        { 
            addr %= (size_t)SRAM_SIZE;
            uint32_t* rom32 = (uint32_t*)sram; 
            rom32[addr >> 2] = value; 
        };

    private: 
        uint8_t sram[SRAM_SIZE];
    };
}