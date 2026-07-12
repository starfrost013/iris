/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    ip2_mmu.hpp: Defines the MMU sitting on the IP2 board
*/

#pragma once 
#include <Iris.hpp>
#include <base/filesystem/filesystem.hpp>
#include <component/addrspace.hpp>
#include <coherent/coherent.hpp>

namespace Iris
{
    //
    // Registers
    //

    #define MMU_START               0x36000000
    #define MMU_END                 0x3F000000

    #define PAGETABLE_MAX_PAGES     (1 << 14) // pagenumber is 13 bits but there might be 2 ptes (one supervisor and one user?)
    #define PAGETABLE_INDEX(x)      0x3B000000 + (x*sizeof(uint32_t))

    #define REG_OS_BASE             0x36000000
    #define REG_STATUS              0x38000000
    #define REG_PARITY              0x39000000
    #define REG_MULTIBUS_PROTECT    0x3A000000
    #define REG_PAGETABLE_BASE      0x3B000000
    #define REG_TEXTDATA_BASE       0x3C000000
    #define REG_TEXTDATA_LIMIT      0x3D000000
    #define REG_STACK_BASE          0x3E000000
    #define REG_STACK_LIMIT         0x3F000000

    /// The coherent extnension
    class CoherentExtensionIP2MMU : public CoherentExtension
    {
    public:
        CoherentExtensionIP2MMU(Component* owner) : CoherentExtension(owner) {}

        void AddUI() override;
    };

    // not sure hwy sgi decided that addresses must be so sparse that bit fucking 24 needed to be the register selector.



    // FOR COMPONENTS, WE DON'T NEED TO BOUNDS CHECK BECAUSE WE ALREADY MAPPED IT!

    class MMU_IP2 : public Component
    {
    public: 
        void Start()
        {
            // map the private ram
            AddrSpaceMapping mapping = AddrSpaceMapping();

            mapping.startAddr = MMU_START;
            mapping.endAddr = MMU_END;
            mapping.component = this;
            AddrSpace::AddMapping(mapping);
        }

        const char* GetName() { return "IRIS 3130 TTL MMU"; };

        uint8_t OnRead8(size_t addr) override;
        uint16_t OnRead16(size_t addr) override;
        uint32_t OnRead32(size_t addr) override;
        void OnWrite8(size_t addr, uint8_t value) override;
        void OnWrite16(size_t addr, uint16_t value) override;
        void OnWrite32(size_t addr, uint32_t value) override; 

        // well probably need to change these so just make them public
        uint32_t osBase;
        uint32_t status;
        uint32_t parity;
        uint32_t multibusProtect;
        uint32_t pagetable[PAGETABLE_MAX_PAGES];
        uint32_t textdataBase;
        uint32_t textdataLimit;
        uint32_t stackBase;
        uint32_t stackLimit;

        MMU_IP2()
        {
            mmuExtension = new CoherentExtensionIP2MMU(this);
            Coherent::RegisterExtension(mmuExtension);
        }

    private: 
        CoherentExtensionIP2MMU* mmuExtension; 

    };


}