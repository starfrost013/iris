#pragma once

#include <Iris.hpp>

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

        // Pointer to member syntax is annoying and complicated >:(
        void (*onRead8)(Component* component);
        void (*onRead16)(Component* component);
        void (*onRead32)(Component* component);
        void (*onWrite8)(Component* component);
        void (*onWrite16)(Component* component);
        void (*onWrite32)(Component* component);
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
 
            void AddMapping(AddrSpaceMapping mapping);
        private: 
            std::unordered_map<size_t, AddrSpaceMapping> mappings;
    };
}