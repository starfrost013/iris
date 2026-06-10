#include <base/component/addrspace.hpp>

namespace Iris
{
    uint8_t AddrSpace::ReadU8()
    {
        return 0;
    }
    
    uint16_t AddrSpace::ReadU16()
    {
        return 0;
    }
    
    uint32_t AddrSpace::ReadU32()
    {
        return 0;
    }
    
    int8_t AddrSpace::ReadS8()
    {
        return 0;
    }
    
    int16_t AddrSpace::ReadS16()
    {
        return 0;
    }
    
    int32_t AddrSpace::ReadS32()
    {
        return 0;
    }

    void AddrSpace::AddMapping(AddrSpaceMapping mapping)
    {
        if (mapping.endAddr > mapping.startAddr)
        {
            Logging::Log(LOGGING_PREFIX_ADDRSPACE, "AddrSpace::AddMapping - mapping.endAddr > mapping.startAddr");
            return;
        }

        auto mappingCount = mappings.count(mapping.startAddr);

        if (mappingCount == 0)
        {
            Logging::Log(LOGGING_PREFIX_ADDRSPACE, "AddrSpace::AddMapping - mapping does not exist");
            return;
        }
        else if (mappingCount > 1)
        {
            Logging::Log(LOGGING_PREFIX_ADDRSPACE, "AddrSpace::AddMapping - mapping already exists");
            return;
        }

        mappings[mapping.startAddr] = mapping;
    }
}