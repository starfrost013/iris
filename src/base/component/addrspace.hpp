#pragma once

#include <Iris.hpp>

namespace Iris
{
    // This class implements an address space mapping.
    // The 
    class AddrSpaceMapping
    {
        // The start address is not stored as is it used as the key in the map.
        size_t endAddr; 
    };

    // Class implementing address space.
    // It is tied to the Machine.
    // By default each machine has
    class AddrSpace
    {
        public:
            // 16-bit - 65536; 24-bit - 16777216; 
            size_t maxAddr;

            uint8_t ReadU8();
            uint16_t ReadU16();
            uint32_t ReadU32();
            int8_t ReadS8();
            int16_t ReadS16();
            int32_t ReadS32();
    };
}