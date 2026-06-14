#pragma once
#include <base/component/addrspace.hpp>

namespace Iris
{
    class Component
    {

    public: 

        int clockSpeed;
        virtual void Start() { };
        virtual void Tick() { };        // ticks at clock speed
        virtual void Shutdown() { };
        
        virtual uint8_t OnRead8(size_t addr) { };
        virtual uint16_t OnRead16(size_t addr) { };
        virtual uint32_t OnRead32(size_t addr) { };
        virtual void OnWrite8(size_t addr, uint8_t value) { };
        virtual void OnWrite16(size_t addr, uint16_t value) { };
        virtual void OnWrite32(size_t addr, uint32_t value) { };
    };
}