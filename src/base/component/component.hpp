#pragma once
#include <base/component/addrspace.hpp>

namespace Iris
{
        class Component
        {

        public: 

            AddrSpace addressSpace; 

            int clockSpeed;
            virtual void Init();
            virtual void Tick();        // tick sat clcoskpeed
            virtual void Shutdown();

            
        };
}
