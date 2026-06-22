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
            addressSpace.maxAddr = 0xFFFFFFFF;  // 32-bit address space by default
            ramCapacity = 16777216;             // Maximum RAM for IRIS 3130
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
        T FindComponentByType()
        {
            for (Component* component : components)
            {
                if (typeid(component) == typeid(T))
                    return (T)component;
            }

            return nullptr; 
        }

        void Start()
        {
            for (Component* component : components)
                component->Start();
        }

        /// @brief Tick each component of the emulation.
        void Tick()
        {
            for (Component* component : components)
            {
                // 0 clockspeed = run AFAP
                // THis may not be a good idea. We may have to add fake cycles.
                if (component->clockSpeed > 0)
                {
                    // maybe microseconds would be better ???
                    auto ns = Chrono_GetTicksNS(Chrono_GetTime());
                    
                    bool run = false; 

                    if (component->delayNs != 0)
                    {
                        auto nsPerTick = (1.0 / (double)component->clockSpeed) * 1000000000; // use maximum precision available

                        if (component->lastTickNs != 0
                        || (ns - component->lastTickNs) > nsPerTick)
                            run = true;
                    }
                    else // slower, delay timing
                    {
                        if ((ns - component->lastTickNs) > component->delayNs)
                            run = true;
                    }

                    if (run)
                    {
                        component->lastTickNs = ns;
                        component->Tick();
                    }
                    
                }
                else // just run
                    component->Tick();

            }
        }

        void Shutdown()
        {
            for (Component* component : components)
                component->Shutdown();
        }

        // not sure if this is a good idea?
        size_t ramCapacity;
        
    private: 
        char name[STRING_MAX_SHORT];
        char friendlyName[STRING_MAX_LONG];         // User's name for this machine

        AddrSpace addressSpace;                     // The address space of the machine. Components can use this to read/write memory.  
    };
};