#include <component/addrspace.hpp>

namespace Iris
{
    // yes, this checks exactly for one. becaue we don't want duplicatges and check for them 
    static bool AddrSpace::MappingExists(size_t addr) { return mappings.count(addr) == 1; }

    uint8_t AddrSpace::ReadU8(size_t addr)
    {
        if (MappingExists(addr))
        {
            AddrSpaceMapping mapping = mappings[addr];
            return mapping.component->OnRead8(addr);
        }
        else
            return 0;
    }
    
    uint16_t AddrSpace::ReadU16(size_t addr)
    {
        if (MappingExists(addr))
        {
            AddrSpaceMapping mapping = mappings[addr];
            return mapping.component->OnRead16(addr);
        }
        else
            return 0;
    }
    
    uint32_t AddrSpace::ReadU32(size_t addr)
    {
        if (MappingExists(addr))
        {
            AddrSpaceMapping mapping = mappings[addr];
            return mapping.component->OnRead32(addr);
        }
        else
            return 0;
    }
    
    int8_t AddrSpace::ReadS8(size_t addr)
    {
        return (int8_t)ReadU8(addr);
    }
    
    int16_t AddrSpace::ReadS16(size_t addr)
    {
        return (int16_t)ReadU16(addr);
    }
    
    int32_t AddrSpace::ReadS32(size_t addr)
    {
        return (int32_t)ReadU32(addr);
    }

    void AddrSpace::AddMapping(AddrSpaceMapping mapping)
    {
        if (mapping.endAddr > mapping.startAddr)
        {
            Logger::Log(LOGGING_PREFIX_ADDRSPACE, "AddrSpace::AddMapping - mapping.endAddr > mapping.startAddr");
            return;
        }

        auto mappingCount = mappings.count(mapping.startAddr);

        if (mappingCount == 0)
        {
            Logger::Log(LOGGING_PREFIX_ADDRSPACE, "AddrSpace::AddMapping - mapping does not exist");
            return;
        }
        else if (mappingCount > 1)
        {
            Logger::Log(LOGGING_PREFIX_ADDRSPACE, "AddrSpace::AddMapping - mapping already exists");
            return;
        }

        mappings[mapping.startAddr] = mapping;
    }

    void AddrSpace::WriteU8(size_t addr, uint8_t value)
    {
        if (MappingExists(addr))
        {
            AddrSpaceMapping mapping = mappings[addr];
            return mapping.component->OnWrite8(addr, value);
        }
    }

    void AddrSpace::WriteU16(size_t addr, uint16_t value)
    {
        if (MappingExists(addr))
        {
            AddrSpaceMapping mapping = mappings[addr];
            return mapping.component->OnWrite16(addr, value);
        }
    }

    void AddrSpace::WriteU32(size_t addr, uint32_t value)
    {
        if (MappingExists(addr))
        {
            AddrSpaceMapping mapping = mappings[addr];
            return mapping.component->OnWrite32(addr, value);
        }
    }

    void AddrSpace::WriteS8(size_t addr, int8_t value)
    {
        WriteU8(addr, (uint8_t)value);
    }

    void AddrSpace::WriteS16(size_t addr, int16_t value)
    {
        WriteU16(addr, (uint16_t)value);
    }

    void AddrSpace::WriteS32(size_t addr, int32_t value)
    {
        WriteU32(addr, (uint32_t)value);
    }
}