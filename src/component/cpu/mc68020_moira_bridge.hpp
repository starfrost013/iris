#pragma once 

#include <Iris.hpp>
#include <coherent/coherent.hpp>
#include <component/component.hpp>
#include <component/cpu/cpu.hpp>
#include <component/cpu/moira/Moira.h>
#include <component/cpu/moira/MoiraDebugger.h>
#include <base/emulation.hpp>

namespace Iris
{
    class MC68020MoiraBridge : public Iris::Lisburn::Moira 
    {
        uint8_t read8(uint32_t addr) const override { return AddrSpace::ReadU8(addr); };
        uint16_t read16(uint32_t addr) const override { return AddrSpace::ReadU16(addr); };
        void write8(uint32_t addr, uint8_t value) const override { return AddrSpace::WriteU8(addr, value); };  
        void write16(uint32_t addr, uint16_t value) const override { return AddrSpace::WriteU16(addr, value); }; 
        
        // Debugger stuff 
        void didReachBreakpoint(uint32_t addr) override { return Coherent::GetSystem()->OnBreakpointHit(); };

        void AddBreakpoint(uint32_t addr) {  };
        void RemoveBreakpoint(uint32_t addr) {   };
    };
}