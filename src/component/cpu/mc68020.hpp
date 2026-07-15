#pragma once

#include <Iris.hpp>
#include <component/component.hpp>
#include <component/cpu/cpu.hpp>
#include <base/emulation.hpp>
#include <component/cpu/mc68020_moira_bridge.hpp>

namespace Iris
{
    #define MOIRA_DISASM_BUF_SIZE    512

    class MC68020DebuggerSystem : public CoherentSystem
    {

    public:
        char* DisasmInstruction(size_t start) override;
        size_t GetPC() override;

        /// Initialise an MC68020 DEbugger System
        MC68020DebuggerSystem(MC68020MoiraBridge* bridge)
        {
            this->moiraCpu = bridge;
        }

        void AddBreakpoint(size_t addr) override
        {
            moiraCpu->AddBreakpoint(addr);
        }

        void RemoveBreakpoint(size_t addr) override
        {
            moiraCpu->RemoveBreakpoint(addr);
        }

    private: 
        char disasmBuf[MOIRA_DISASM_BUF_SIZE] = {0};
        MC68020MoiraBridge* moiraCpu;
    };

    class MC68020 : public ComponentCPU
    {
        #define MC68020_CACHE_SIZE      256
        #define MC68020_NUM_OPCODES     256

        #define MC68020_NUM_LINES       16
        #define MC68020_NUM_DATA_REGS   8
        #define MC68020_NUM_ADDR_REGS   8   // A7 is the s


    public: 

        MC68020MoiraBridge moiraCpu;
        // MMU & FPU are off-chip. 68881 + 68882.
        
        //
        // METHODS
        // 

        uint32_t GetClockSpeed() override { return 16777000; }; 

        void Start() override;
        void Tick() override;
        void Shutdown() override; 

        /// @brief get the name of this component. immutable const char*.
        const char* GetName() { return "Motorola MC68020 CPU (Moira/Lisburn)"; };
    private:
        MC68020DebuggerSystem* system; 

    };
}