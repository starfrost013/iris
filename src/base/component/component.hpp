#pragma once
#include <base/component/addrspace.hpp>

namespace Iris
{
    class Component
    {

    public: 

        AddrSpace addressSpace; 

        int clockSpeed;
        virtual void Init() = 0;
        virtual void Tick() = 0;        // ticks at clock speed
        virtual void Shutdown() { };

        
    };
}
