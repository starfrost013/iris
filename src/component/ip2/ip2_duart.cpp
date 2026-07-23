#include <component/ip2/ip2_duart.hpp>

namespace Iris
{
    uint8_t DUART68681::OnRead8(size_t addr)
    {
        addr = addr & DUART_NUM_REGS;
        auto duartId = GetDuartIONum(addr);
        Logger::Log(DUART_LOG_PREFIX, std::format("DUART{} read from addr {:x}", duartId, addr).c_str(), LogChannels::Debug);
        return 0xFF;
    }

    uint16_t DUART68681::OnRead16(size_t addr)
    {
        addr = addr & DUART_NUM_REGS;
        auto duartId = GetDuartIONum(addr);
        Logger::Log(DUART_LOG_PREFIX, std::format("DUART{} read from addr {:x}", duartId, addr).c_str(), LogChannels::Debug);
        return 0xFFFF;
    }

    uint32_t DUART68681::OnRead32(size_t addr)
    {
        addr = addr & DUART_NUM_REGS;
        auto duartId = GetDuartIONum(addr);
        Logger::Log(DUART_LOG_PREFIX, std::format("DUART{} read from addr {:x}", duartId, addr).c_str(), LogChannels::Debug);
        return 0xFFFFFFFF;
    }

    void DUART68681::OnWrite8(size_t addr, uint8_t value)
    {
        addr = addr & DUART_NUM_REGS;
        auto duartId = GetDuartIONum(addr);
        Logger::Log(DUART_LOG_PREFIX, std::format("DUART{} write to addr {:x}", duartId, addr).c_str(), LogChannels::Debug);
    }

    void DUART68681::OnWrite16(size_t addr, uint16_t value)
    {
        addr = addr & DUART_NUM_REGS;
        auto duartId = GetDuartIONum(addr);
        Logger::Log(DUART_LOG_PREFIX, std::format("DUART{} write to addr {:x}", duartId, addr).c_str(), LogChannels::Debug);
    }

    void DUART68681::OnWrite32(size_t addr, uint32_t value)
    {
        addr = addr & DUART_NUM_REGS;
        auto duartId = GetDuartIONum(addr);
        Logger::Log(DUART_LOG_PREFIX, std::format("DUART{} rwrite to addr {:x}", duartId, addr).c_str(), LogChannels::Debug);
    }
}