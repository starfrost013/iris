#pragma once

#include <vector>
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
    };
};