#pragma once

#include <Iris.hpp>
#include <component/addrspace.hpp>
#include <component/component.hpp>

namespace Iris
{
    class Machine
    {
        #define COMPONENTS_INITIAL_RESERVED     16  
        #define MACHINE_CONFIG_PATH             "Machine.cfg"
        #define LOG_PREFIX_EMU_MACHINE          "Emulation - Machine"

        std::vector<Component*> components;

    public:
        Machine()
        {
            components.reserve(COMPONENTS_INITIAL_RESERVED);
            AddrSpace::maxAddr = 0xFFFFFFFF;                    // 32-bit address space by default
            ramCapacity = 16777216;                             // Maximum RAM for IRIS 3130
        }

        char* GetName() { return name; };
        void SetName(char* name) { strncpy(name, this->name, STRING_MAX_SHORT ); };
        char* GetFriendlyName() { return friendlyName; };
        void SetFriendlyName(char* friendlyName) { strncpy(friendlyName, this->name, STRING_MAX_SHORT ); };

        template <std::derived_from<Component> T>   
        T* AddComponent()  
        {
            T* newComponent = new T();

            // test. probably slow.
            Logger::Log(LOG_PREFIX_EMU_MACHINE, std::format("Added component {}", newComponent->GetName()).c_str(), LogChannels::Debug);
            components.push_back(newComponent);

            return newComponent;
        } 

        template <std::derived_from<Component> T>
        T* FindComponentByType()
        {
            for (Component* component : components)
            {
                T* derived = dynamic_cast<T*>(component);
                if (derived)
                    return derived;
            }

            return nullptr;
        }

        /// @brief Start the machine.
        void Start();

        /// @brief Tick each component of the emulation.
        void Tick();

        /// @brief Single step the machine.
        void SingleStep()
        {
            // run tick once
            Tick();
        }

        void Shutdown();

        // not sure if this is a good idea?
        size_t ramCapacity;

        // Setters for private fields
    private: 
        char name[STRING_MAX_SHORT] = {0};          // Emulator's name for this machine
        char friendlyName[STRING_MAX_LONG] = {0};   // User's name for this machine

        AddrSpace addressSpace;                     // The address space of the machine. Components can use this to read/write memory.  
    };
};