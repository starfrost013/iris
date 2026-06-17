#include <cpu/coherent/coherent.hpp>

namespace Iris
{
    //
    // FUll-specialistaion templates for certain things
    //

    // Convering registers to decimal 

    template<> void CoherentSystem::Register<uint8_t>::TextifyDecimal(uint8_t value, char* buf) { snprintf(buf, sizeof(buf), "%d", value); };
    template<> void CoherentSystem::Register<int8_t>::TextifyDecimal(int8_t value, char* buf) { snprintf(buf, sizeof(buf), "%d", value); };
    template<> void CoherentSystem::Register<uint16_t>::TextifyDecimal(uint16_t value, char* buf) { snprintf(buf, sizeof(buf), "%d", value); };
    template<> void CoherentSystem::Register<int16_t>::TextifyDecimal(int16_t value, char* buf) { snprintf(buf, sizeof(buf), "%d", value); };
    template<> void CoherentSystem::Register<uint32_t>::TextifyDecimal(uint32_t value, char* buf) { snprintf(buf, sizeof(buf), "%d", value); };
    template<> void CoherentSystem::Register<int64_t>::TextifyDecimal(int64_t value, char* buf) { snprintf(buf, sizeof(buf), "%ld", value); };
    template<> void CoherentSystem::Register<uint64_t>::TextifyDecimal(uint64_t value, char* buf) { snprintf(buf, sizeof(buf), "%ld", value); };
    template<> void CoherentSystem::Register<float>::TextifyDecimal(float value, char* buf) { snprintf(buf, sizeof(buf), "%f", value); };
    template<> void CoherentSystem::Register<double>::TextifyDecimal(double value, char* buf) { snprintf(buf, sizeof(buf), "%f", value); };

    template<> void CoherentSystem::Register<uint8_t>::TextifyHex(uint8_t value, char* buf) { snprintf(buf, sizeof(buf), "%x", value); };
    template<> void CoherentSystem::Register<int8_t>::TextifyHex(int8_t value, char* buf) { snprintf(buf, sizeof(buf), "%x", value); };
    template<> void CoherentSystem::Register<uint16_t>::TextifyHex(uint16_t value, char* buf) { snprintf(buf, sizeof(buf), "%x", value); };
    template<> void CoherentSystem::Register<int16_t>::TextifyHex(int16_t value, char* buf) { snprintf(buf, sizeof(buf), "%x", value); };
    template<> void CoherentSystem::Register<uint32_t>::TextifyHex(uint32_t value, char* buf) { snprintf(buf, sizeof(buf), "%x", value); };
    template<> void CoherentSystem::Register<int64_t>::TextifyHex(int64_t value, char* buf) { snprintf(buf, sizeof(buf), "%lx", value); };
    template<> void CoherentSystem::Register<uint64_t>::TextifyHex(uint64_t value, char* buf) { snprintf(buf, sizeof(buf), "%lx", value); };

    //
    // Core
    //

    
}