// Motion Emulator
// Copyright (C) 2026 starfrost
//
// Memory.hpp: Defines the actual system RAM on the IRIS 3130.

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

    private: 
        size_t GetRamCapacity() { return Emulation::GetMachine().ram; };
    }
}