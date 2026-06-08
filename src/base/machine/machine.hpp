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

        void Init()
        {
            for (Component& component : components)
                component.Init();
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

    private: 
        char name[STRING_MAX_SHORT];
        char friendlyName[STRING_MAX_LONG];         // User's name
    };
};