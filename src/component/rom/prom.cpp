#include <component/rom/prom.hpp>

namespace Iris
{
    Cvar* promPath;
    Cvar* promSize;
    
    void PROM::Start()
    {
        promPath = Cvar::Set("promPath", "./roms/iris3130/ip2/prom_3.0.11.bin");
        promSize = Cvar::Set("promSize", "98304");

        rom = new uint8_t[(size_t)promSize->GetValue()];
    }

    void PROM::Shutdown()
    {
        delete rom;
    }

}