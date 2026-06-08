#pragma once
#include <base/component/addrspace.hpp>

namespace Iris
{
    class Component
    {

    public: 

        AddrSpace addressSpace; 

        int clockSpeed;
        virtual void Init() { };
        virtual void Tick() { };        // ticks at clock speed
        virtual void Shutdown() { };

        
    };
}
