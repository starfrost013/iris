#include <Iris.hpp>
#include <base/filesystem/filesystem.hpp>
#include <component/addrspace.hpp>

namespace Iris
{
    extern Cvar* promPath;
    extern Cvar* promSize;

    #define PROM_START_ADDRESS      0x30000000
    #define PROM_LOG_PREFIX         "EmuCore - ROM"

    // FOR COMPONENTS, WE DON'T NEED TO BOUNDS CHECK BECAUSE WE ALREADY MAPPED IT!

    class PROM : public Component
    {
    public: 
        void Start() override;
        void Shutdown() override;

        const char* GetName() { return "IRIS 3130 System PROM"; };

        uint8_t OnRead8(size_t addr) override 
        { 
            return (rom[addr]); 
        };

        uint16_t OnRead16(size_t addr) override 
        { 
            uint16_t* rom16 = (uint16_t*)rom; 
            return rom16[addr >> 1]; 
        };

        uint32_t OnRead32(size_t addr) override 
        { 
            uint32_t* rom32 = (uint32_t*)rom; 
            return rom32[addr >> 2]; 
        };

        void OnWrite8(size_t addr, uint8_t value) { rom[addr % (size_t)promSize->GetValue()] = value; };

        void OnWrite16(size_t addr, uint16_t value) 
        { 
            uint16_t* rom16 = (uint16_t*)rom; 
            rom16[addr >> 1] = value; 
        };

        void OnWrite32(size_t addr, uint32_t value)
        { 
            uint32_t* rom32 = (uint32_t*)rom; 
            rom32[addr >> 2] = value; 
        };

    private: 
        uint8_t* rom;
    };
}