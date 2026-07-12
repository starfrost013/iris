#include <component/ip2/ip2_duart.hpp>

namespace Iris
{
    uint8_t DUART68681::OnRead8(size_t addr)
    {
        return 0xFF;
    }

    uint16_t DUART68681::OnRead16(size_t addr)
    {
        return 0xFFFF;
    }

    uint32_t DUART68681::OnRead32(size_t addr)
    {
        return 0xFFFFFFFF;
    }

    void DUART68681::OnWrite8(size_t addr, uint8_t value)
    {

    }

    void DUART68681::OnWrite16(size_t addr, uint16_t value)
    {

    }

    void DUART68681::OnWrite32(size_t addr, uint32_t value)
    {

    }
}