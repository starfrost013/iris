#pragma once
#include <Iris.hpp>
#include <base/component/component.hpp>

namespace Iris
{
    class CPU_MC68020 : Component
    {
        #define MC68020_CACHE_SIZE      256
        // We decode per line.
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
        enum ExceptionVectors
        {

        };

        // Bits 15...12 of byte 0o f the opcode/SEA-OW
        enum InstructionLines
        {
            Bit = 0,
            MoveByte = 1,
            MoveWord = 2,
            MoveLong = 3,
            Misc = 4,
            AddQSubQ = 5, // also scc, dbcc, trapcc
            Branch = 6,   
            MoveQ = 7,
            OrDivSbcd = 8,
            SubSubx = 9,
            LineAEmulator = 0x0A, // Unassignerd
            CmpEor = 0x0B,
            AndMul = 0x0C, // also ABXC, EXG
            AddAddx = 0x0D,
            ShiftRotateBitfield = 0x0E,
            Coprocessor = 0x0F,
        };

        // The 16 instruction 'lines'

        struct InstructionLine
        {
            uint8_t cycles;
            void (CPU_MC68020::*run)();
        }; 

        static constexpr InstructionLine instructions[] = {

        };

    public: 

        // General purpose register
        // using an array makes it easier to analyse
        uint32_t dataRegs[MC68020_NUM_DATA_REGS];

        // Address registers
        uint32_t addrRegs[MC68020_NUM_ADDR_REGS];

        uint32_t pc;            // Program counter

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


    private:

        uint32_t cache[MC68020_CACHE_SIZE >> 2];
        
        void Execute(uint16_t opcode); 

    };
}