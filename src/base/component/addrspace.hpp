#pragma once

#include <Iris.hpp>

namespace Iris
{
    // Class implementing address space.
    // It is tied to the Component (Unlike in volt when it was its own thing).
    class AddrSpace
    {
        public:
            // 16-bit - 65536; 24-bit - 16777216; 
            int maxAddr;

            // CoreSystem
            uint8_t ReadU8();
            uint16_t ReadU16();
            uint32_t ReadU32();
            int8_t ReadS8();
            int16_t ReadS16();
            int32_t ReadS32();
    };
}