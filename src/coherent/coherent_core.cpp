#include <coherent/coherent.hpp>

namespace Iris
{
    //
    // FUll-specialistaion templates for certain things
    //

    // Convering registers to decimal and hex formats

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
    // Coherent class
    //

    /// @brief a C trampoline. since the logging system is portable across projects which may use various paradigms, its function poitners use the lowest common 
    /// denominator (ie a function pointer.)
    /// @param str 
    void Coherent_CTrampolineForLog(const char* str)
    {
        Coherent::AddTextToLogWindowBuffer(str);
    }

    void Coherent::Init()
    {
        Logger::Log(COHERENT_LOG_PREFIX, COHERENT_VERSION " initialised");
        Logger::settings.SetPostLogFunction(Coherent_CTrampolineForLog);

        initialised = true; 
    }

    void Coherent::ExecStart()
    {

    }

    void Coherent::ExecEnd()
    {

    }

    void Coherent::Enter()
    {
        active = true;
    }


    void Coherent::Leave()
    {   
        active = false; 
    }
    
    void Coherent::Shutdown()
    {
        Logger::settings.SetPostLogFunction(nullptr);
    }

    //
    // Extension registration
    //

    void Coherent::RegisterExtension(CoherentExtension* extension)
    {

    }


}