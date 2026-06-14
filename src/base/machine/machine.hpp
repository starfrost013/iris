#pragma once

#include <Iris.hpp>
#include <base/component/component.hpp>

namespace Iris
{
    class Machine
    {
        #define COMPONENTS_INITIAL_RESERVED     16  

        std::vector<Component> components;

    public:
        Machine()
        {
            components.reserve(COMPONENTS_INITIAL_RESERVED);
            addressSpace.maxAddr = 0xFFFFFFFF; // 32-bit address space by default
        }

        char* GetName() { return name; };
        void SetName(char* name) { strncpy(name, this->name, STRING_MAX_SHORT ); };
        char* GetFriendlyName() { return friendlyName; };
        void SetFriendlyName(char* friendlyName) { strncpy(friendlyName, this->name, STRING_MAX_SHORT ); };

        template <std::derived_from<Component> T>   
        void AddComponent(T component)  
        {
            components.push_back(component);
        } 

        template <std::derived_from<Component> T>
        T FindComponentByType()
        {
            for (Component& component : components)
            {
                if (typeid(component) == typeid(T))
                    return (T)component;
            }

            return nullptr; 
        }

        void Start()
        {
            for (Component& component : components)
                component.Start();
        }

        void Tick()
        {
            for (Component& component : components)
                component.Tick();
        }

        void Shutdown()
        {
            for (Component& component : components)
                component.Shutdown();
        }


        // not sure if this is a good idea?
        size_t ram;
        

    private: 
        char name[STRING_MAX_SHORT];
        char friendlyName[STRING_MAX_LONG];         // User's name for this machine

        AddrSpace addressSpace;                     // The address space of the machine. Components can use this to read/write memory.  
    };
};