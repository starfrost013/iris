/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    ip2_duart.hpp: Emulates two SCN68681 UARTs mapped at 32000000 and 32800000 respectively

    Largely adapted from the MAME emulation:
    https://github.com/mamedev/mame/blob/master/src/devices/machine/mc68681.h
    https://github.com/mamedev/mame/blob/master/src/devices/machine/mc68681.cpp
*/

#pragma once 
#include <Iris.hpp>
#include <base/filesystem/filesystem.hpp>
#include <component/addrspace.hpp>
#include <coherent/coherent.hpp>

namespace Iris
{
    #define DUART0_START                    0x32000000 
    #define DUART1_START                    0x32800000 
    #define DUART_NUM_REGS                  16
    #define DUART_NUM_INPUT_PORTS           7

    // There is a small fifo in the 68681 for receiving bits
    #define DUART_FIFO_SIZE                 3

    #define DUART_NUM_CHANNELS              2

    //
    // Registers
    //

    // Universal registers (read/write) [bit3 selects A/B]
    #define DUART_MODE_A                            0x0             // 0x0: [Read/Write] Mode Register A
    #define DUART_MODE_B                            0x8             // 0x8: [Read/Write] Mode Register B
    #define DUART_INTERRUPT_VECTOR                  0xC             // 0xC: [Read] Interrupt Vector

    // Read registers
    #define DUART_READ_STATUS_A                     0x1             // 0x1: [Read] Status Register A
    #define DUART_READ_BRG_TEST                     0x2             // 0x2: [Read] BRG Test
    #define DUART_READ_RX_HOLD_A                    0x3             // 0x3: [Read] Rx Holding Register A
    #define DUART_READ_INPUT_PORT_CHANGE            0x4             // 0x4: [Read] Input Port Change
    #define DUART_READ_INTERRUPT_STATUS             0x5             // 0x5: [Read] Interrupt Status
    #define DUART_READ_COUNTER_UPPER                0x6             // 0x6: [Read] Counter/Timer Upper Byte
    #define DUART_READ_COUNTER_LOWER                0x7             // 0x7: [Read] Counter/Timer Lower Byte
    #define DUART_READ_STATUS_B                     0x9             // 0x9: [Read] Status Register B
    #define DUART_READ_1X16X                        0xA             // 0xA: [Read] 1x/16x Test
    #define DUART_READ_RX_HOLD_B                    0xB             // 0xB: [Read] Rx Holding Register B
    #define DUART_READ_INPUT_PORTS                  0xD             // 0xD: [Read] Input Ports IP0-IP6
    #define DUART_READ_START_COUNTER_CMD            0xE             // 0xE: [Read] Start Counter Command
    #define DUART_READ_STOP_COUNTER_CMD             0xF             // 0xF: [Read] Stop Counter Command

    // Write registers
    #define DUART_WRITE_CLOCKSEL_A                  0x1             // 0x1: [Write] Clock Select Register A
    #define DUART_WRITE_COMMAND_A                   0x2             // 0x2: [Write] Command Register A
    #define DUART_WRITE_TX_HOLD_A                   0x3             // 0x3: [Write] Tx Holding Register A
    #define DUART_WRITE_AUX_CONTROL                 0x4             // 0x4: [Write] Aux Control
    #define DUART_WRITE_INTERRUPT_MASK              0x5             // 0x5: [Write] Interrupt Mask
    #define DUART_WRITE_COUNTER_UPPER               0x6             // 0x6: [Write] Counter/Timer Preset Upper Byte
    #define DUART_WRITE_COUNTER_LOWER               0x7             // 0x7: [Write] Counter/Timer Preset Lower Byte
    #define DUART_WRITE_CLOCKSEL_B                  0x9             // 0x9: [Write] Clock Select Register B
    #define DUART_WRITE_COMMAND_B                   0xA             // 0xA: [Write] Command Register B
    #define DUART_WRITE_TX_HOLD_B                   0xB             // 0xB: [Write] Tx Holding Register B
    #define DUART_WRITE_OUTPUT_PORT_CONF            0xD             // 0xD: [Write] Output Port Configuration
    #define DUART_WRITE_SET_OUTPUT_PORT_BITS_CMD    0xE             // 0xE: [Write] Set Output Port Bits
    #define DUART_WRITE_RESET_OUTPUT_PORT_BITS_CMD  0xF             // 0xF: [Write] Reset Output Port Bits

    #define DUART_LOG_PREFIX                        "Emulation - IP2 DUART"
    // FOR COMPONENTS, WE DON'T NEED TO BOUNDS CHECK BECAUSE WE ALREADY MAPPED THE RIGHT SIZE!

    class DUART68681 : public Component
    {

    public: 
        void Start()
        {
            // map the DUARTs
            AddrSpaceMapping mapping0 = AddrSpaceMapping();

            mapping0.startAddr = DUART0_START;
            mapping0.endAddr = DUART0_START + DUART_NUM_REGS;
            mapping0.component = this;

            AddrSpaceMapping mapping1 = AddrSpaceMapping();

            mapping1.startAddr = DUART1_START;
            mapping1.endAddr = DUART1_START + DUART_NUM_REGS;
            mapping1.component = this;

            AddrSpace::AddMapping(mapping0);
            AddrSpace::AddMapping(mapping1);
        }

        const char* GetName() { return "Dual Signetics SCN68681 UART (IP2/U130 + IP2/U131)"; };

        // todo: we need to do something
        int GetDuartIONum(size_t addr) { return (addr & 0x800000) ? 1 : 0; }

        uint8_t OnRead8(size_t addr) override;
        uint16_t OnRead16(size_t addr) override;
        uint32_t OnRead32(size_t addr) override;
        void OnWrite8(size_t addr, uint8_t value) override;
        void OnWrite16(size_t addr, uint16_t value) override;
        void OnWrite32(size_t addr, uint32_t value) override; 

        // there are two uart channels
        struct UARTChannel
        {
            int32_t baudRateRX, baudRateTX;
            uint8_t command;
            uint8_t clocksel;
            uint8_t mode1, mode2;
            uint8_t modeRegCurrent; 

            // transmit/receive
            uint8_t txEnabled, rxEnabled;
            uint8_t txHold, rxHold; 
            uint8_t txData, rxData;

            bool txBreak;
            uint8_t txBitsTransmitted;
            uint8_t txPrescale, rxPrescale;
            uint8_t rxFifo[DUART_FIFO_SIZE + 1];
        };

        struct DUART
        {   
            uint8_t isr;                // interrupt status register [Read]
            uint8_t ivr;                // interrupt vector register [Read/Write]
            uint8_t imr;                // interrupt mask
            uint16_t counter;           // counter/timer
            uint16_t counterPreset;     // counter/timer preset value

            // Input ports
            uint8_t inputPorts[DUART_NUM_INPUT_PORTS] = {0};
            UARTChannel channels[DUART_NUM_CHANNELS] = {0};

        };

        DUART uarts[2] = {0};

    };
}