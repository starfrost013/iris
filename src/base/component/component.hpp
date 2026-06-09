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

        
    };
}
