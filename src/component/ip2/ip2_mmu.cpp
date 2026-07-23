/*
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    ip2_mmu.cpp: Implements the MMU sitting on the IP2 board
*/

#include <component/ip2/ip2_mmu.hpp>

namespace Iris
{
    uint8_t MMU_IP2::OnRead8(size_t addr)
    {
        uint32_t ret = OnRead32(addr);
        return READ_32TO8(ret, addr);
    }

    uint16_t MMU_IP2::OnRead16(size_t addr)
    {
        uint32_t ret = OnRead32(addr);
        return READ_32TO16(ret, addr);
    }

    uint32_t MMU_IP2::OnRead32(size_t addr)
    {
        uint32_t ret;

        switch (addr)
        {
        case REG_OS_BASE:
            ret = osBase;
            break;
        case REG_STATUS:
            ret = status;
            break;
        case REG_PARITY:
            ret = parity;
            break;
        case REG_MULTIBUS_PROTECT:
            ret = multibusProtect;
            break;
        case REG_PAGETABLE_BASE ... PAGETABLE_INDEX(PAGETABLE_MAX_PAGES):
            ret = pagetable[(addr - REG_PAGETABLE_BASE) >> 2];
            break;
        case REG_TEXTDATA_BASE:
            ret = textdataBase;
            break;
        case REG_TEXTDATA_LIMIT:
            ret = textdataLimit;
            break;
        case REG_STACK_BASE:
            ret = stackBase;
            break;
        case REG_STACK_LIMIT:
            ret = stackLimit;
            break;
        default:
            Logger::Log(LOG_PREFIX_IP2MMU, std::format("Invalid IP2 MMU read at address 0x{:x}", addr).c_str(), LogChannels::Warning);
            ret = 0xFF;
            return ret;
        }

        //Logger::Log(LOG_PREFIX_IP2MMU, std::format("IP2 MMU read 0x{:x} from address 0x{:x} (check debug window)", ret, addr).c_str(), LogChannels::Message);
        return ret;
    }

    void MMU_IP2::OnWrite8(size_t addr, uint8_t value)
    {
        uint32_t newVal = OnRead32(addr);
        WRITE_32TO8(newVal, value, addr);
        OnWrite32(addr, newVal);
    }

    void MMU_IP2::OnWrite16(size_t addr, uint16_t value)
    {
        uint32_t newVal = OnRead32(addr);

        WRITE_32TO16(newVal, value, addr);

        // newVal &= (~0xFFFF << (addr & 3) << 3);
        // newVal |= (value << ((addr & 3) << 3));

        OnWrite32(addr, newVal);
    }

    void MMU_IP2::OnWrite32(size_t addr, uint32_t value)
    {
        switch (addr)
        {
        case REG_OS_BASE:
            osBase = value;
            break;
        case REG_STATUS:
            status = value;
            break;
        case REG_PARITY:
            parity = value;
            break;
        case REG_MULTIBUS_PROTECT:
            multibusProtect = value;
            break;
        case REG_PAGETABLE_BASE ... PAGETABLE_INDEX(PAGETABLE_MAX_PAGES):
            pagetable[(addr - REG_PAGETABLE_BASE) >> 2] = value;
            break;
        case REG_TEXTDATA_BASE:
            textdataBase = value;
            break;
        case REG_TEXTDATA_LIMIT:
            textdataLimit = value;
            break;
        case REG_STACK_BASE:
            stackBase = value;
            break;
        case REG_STACK_LIMIT:
            stackLimit = value;
            break;
        default:
            Logger::Log(LOG_PREFIX_IP2MMU, std::format("Invalid IP2 MMU write at address 0x{:x}", addr).c_str(), LogChannels::Warning);
            return;
        }

        //Logger::Log(LOG_PREFIX_IP2MMU, std::format("IP2 MMU write 0x{:x} to address 0x{:x} (check debug window)", value, addr).c_str(), LogChannels::Message);
        return;
    }
}