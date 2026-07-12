/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    ip2_duart.hpp: Emulates two SCN68681 UARTs mapped at 32000000 and 32800000
*/

#pragma once 
#include <Iris.hpp>
#include <base/filesystem/filesystem.hpp>
#include <component/addrspace.hpp>
#include <coherent/coherent.hpp>

namespace Iris
{
    #define DUART0_START       0x32000000 
    #define DUART1_START       0x32800000 
    #define DUART_NUM_REGS     16

    //
    // Registers
    //

    // FOR COMPONENTS, WE DON'T NEED TO BOUNDS CHECK BECAUSE WE ALREADY MAPPED IT!

    class DUART68681 : public Component
    {
    public: 
        void Start()
        {
            // map the DUARTs
            AddrSpaceMapping mapping0 = AddrSpaceMapping();

            mapping0.startAddr = DUART0_START;
            mapping0.endAddr = DUART0_START + DUART_NUM_REGS;
            mapping0.component = this;

            AddrSpaceMapping mapping1 = AddrSpaceMapping();

            mapping1.startAddr = DUART1_START;
            mapping1.endAddr = DUART1_START + DUART_NUM_REGS;
            mapping1.component = this;

            AddrSpace::AddMapping(mapping0);
            AddrSpace::AddMapping(mapping1);
        }

        const char* GetName() { return "Dual Signetics SCN68681 UART (IP2/U130 + IP2/U131)"; };

        // todo: we need to do something
        int GetDuartIONum(size_t addr) { return (addr >= DUART1_START) ? 1 : 0; }

        uint8_t OnRead8(size_t addr) override;
        uint16_t OnRead16(size_t addr) override;
        uint32_t OnRead32(size_t addr) override;
        void OnWrite8(size_t addr, uint8_t value) override;
        void OnWrite16(size_t addr, uint16_t value) override;
        void OnWrite32(size_t addr, uint32_t value) override; 

        struct DUART
        {

        };

    };
}