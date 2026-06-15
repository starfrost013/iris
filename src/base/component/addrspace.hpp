#pragma once

#include <Iris.hpp>
#include <base/component/component.hpp>


#define LOGGING_PREFIX_ADDRSPACE        "EmuCore - Addressing"

namespace Iris
{
    // This class implements an address space mapping.
    // The 
    class AddrSpaceMapping
    {
    public: 
        // The start address is used as the key in the map.
        size_t startAddr;
        size_t endAddr; 

        Component* component;
    };

    // Class implementing address space.
    // It is tied to the Machine.
    // By default each machine has
    class AddrSpace
    {
        public:
            // 16-bit - 65536; 24-bit - 16777216; 
            size_t maxAddr;

            uint8_t ReadU8(size_t addr);
            uint16_t ReadU16(size_t addr);
            uint32_t ReadU32(size_t addr);
            int8_t ReadS8(size_t addr);
            int16_t ReadS16(size_t addr);
            int32_t ReadS32(size_t addr);
 
            void WriteU8(size_t addr, uint8_t value);
            void WriteU16(size_t addr, uint16_t value);
            void WriteU32(size_t addr, uint32_t value);
            void WriteS8(size_t addr, int8_t value);
            void WriteS16(size_t addr, int16_t value);
            void WriteS32(size_t addr, int32_t value);
 
            void AddMapping(AddrSpaceMapping mapping);
        private: 
            std::unordered_map<size_t, AddrSpaceMapping> mappings;

            bool MappingExists(size_t addr);

    };
}