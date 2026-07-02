#pragma once
#include <Iris.hpp>
#include <component/component.hpp>
#include <base/emulation.hpp>

namespace Iris
{
    class CPU_MC68020 : public Component
    {
        #define MC68020_CACHE_SIZE      256
        #define MC68020_NUM_OPCODES     256

        #define MC68020_NUM_LINES       16
        #define MC68020_NUM_DATA_REGS   8
        #define MC68020_NUM_ADDR_REGS   7   // 8 is the sp

        // Defines the source of an instruction while executing it
        enum InstructionSource
        {   
            Cache = 0,
            AddressSpace = 1,
        }; 

        // Oh crap! we have a big problem (add to vbt register)
        // 00 / 04 are used for the initial sp and pc respectively.
        // SHift left by 2 to get the address to execute
        enum ExceptionVectors
        {  
            BusError = 2,
            AddressError = 3,
            IllegalInstruction = 4,
            ZeroDivide = 5,
            ChkInstruction = 6,
            TrapInstruction = 7,
            PrivilegeViolation = 8,
            Line1010 = 9,
            Line1111 = 10,
            Reserved = 11,
            CoprocViolation = 12,
            FormatError = 13,
            UninitialisedInterrupt = 14,
            Reserved1Start = 15,
            Reserved1End = 23,
            SpuriousInterrupt = 24,
            L1AutoVector = 25,
            L2AutoVector = 26,
            L3AutoVector = 27,
            L4AutoVector = 28,
            L5AutoVector = 29,
            L6AutoVector = 30,
            L7AutoVector = 31,
            TrapStart = 32,                     // TRAP 0
            TrapEnd = 47,                       // TRAP 15
            Reserved2Start = 48,
            Reserved2End = 63,
            UserStart = 64,
            UserEnd = 255,                      // UDV 192
        };

        // The 16 instruction 'lines'

        struct Instruction
        {
            uint16_t opcode;                    // The Single Effective Address Operation Word (SEA OW!)
            uint16_t cycles;                    // cycle count
            void (CPU_MC68020::*run)();
        }; 


    public: 

        // General purpose register
        // using an array makes it easier to analyse
        uint32_t dataRegs[MC68020_NUM_DATA_REGS];

        // Address registers
        uint32_t addrRegs[MC68020_NUM_ADDR_REGS];

        uint8_t PCPeek8() { return AddrSpace::ReadU8(pc); };
        uint16_t PCPeek16() { return AddrSpace::ReadU16(pc); };
        uint32_t PCPeek32() { return AddrSpace::ReadU32(pc); };

        uint8_t PCRead8() { uint8_t ret = PCPeek8(); pc++; return ret; };
        uint16_t PCRead16() { uint16_t ret = PCPeek16(); pc += 2; return ret; };
        uint32_t PCRead32() { uint32_t ret = PCPeek32(); pc += 4; return ret; };
        
        // Flags:
        // 000XNZVC - eXtend, Negative, Zero, Overflow, Carry 
        uint8_t ccr;            // Flags

        // Technically upper byte of CCR, but easier to treat it like this.
        // 7...6: Trace Enable
        // 5: Mode (Supervisor / User)
        // 4: Master / Interrupt Stack
        // 3: 0
        // 2...0: Interrupt Mask  
        uint8_t status;         // Status 

        uint32_t usp, ssp;      // User Stack & SUpervisor Stack Pointer
        uint32_t isp;           // Interrupt Stack Pointer

        uint32_t vbr;           // Base of interrupt vector table

        uint8_t sfc, dfc;       // SFC = Alternate Function, DFC = Alternate Code
        // Cache

        uint32_t cacr;          // Cache Control
        uint32_t caar;          // Cache Address
        uint32_t cahr;          // Cache Holding
        
        // MMU & FPU are off-chip. 68881 + 68882.
        
        //
        // METHODS
        // 

        int clockSpeed = 16777000;  // 16.7MHz (MC68020-BC16)

        void Start() override;
        void Tick() override;
        void Shutdown() override; 

        /// @brief get the name of this component. immutable const char*.
        const char* GetName() { return "Motorola MC68020 CPU"; };
    private:
        uint32_t pc;            // Program counter

        uint32_t cache[MC68020_CACHE_SIZE >> 2];

        void Op_Test() { };

        inline static const std::unordered_map<uint16_t, Instruction> instructions =
        {
            { 0, { 0b0000'0000'0000'0000, 1, &CPU_MC68020::Op_Test } },
        };

        CoherentSystem* system; 

    };
}