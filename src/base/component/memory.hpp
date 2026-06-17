// Motion Emulator
// Copyright (C) 2026 starfrost
//
// Memory.hpp: Defines the actual system RAM on the IRIS 3130.
// 
// On the IRIS 3000 (Juniper) machines, memory is implemented as up to 4 IM1 (Inhouse Memory 1) boards, with 2 or 4 MBytes each for a max of 16mb ram.
// The MMU has 13-bit page numbers and 4kb pages for a theoretical maximum of 32 MB system RAM.

#pragma once
#include <Iris.hpp>
#include <base/component/component.hpp>

namespace Iris
{
 
    class Memory : public Component
    {
        uint8_t* ram;
        
        void Start() override;

    public: 
        uint8_t OnRead8(size_t addr) override; 
        uint16_t OnRead16(size_t addr) override; 
        uint32_t OnRead32(size_t addr) override;
        void OnWrite8(size_t addr, uint8_t value) override; 
        void OnWrite16(size_t addr, uint16_t value) override;
        void OnWrite32(size_t addr, uint32_t value) override;

        /// @brief get the name of this component. immutable const char*.
        const char* GetName() { return "System RAM"; };

    private: 
        size_t GetRamCapacity() { return Emulation::GetMachine().ramCapacity; };
    };
}