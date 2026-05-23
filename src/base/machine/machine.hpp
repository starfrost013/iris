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

    private: 
        char name[STRING_MAX_SHORT];
        char friendlyName[STRING_MAX_LONG];         // User's name
    };
};