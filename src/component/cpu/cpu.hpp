#include <Iris.hpp>
#include <component/component.hpp>

namespace Iris
{
    /// @brief Base class for components that implement a CPU
    class ComponentCPU : public Component
    {
    public:
        virtual uint32_t GetClockSpeed() { return 0; }; 

        /// @brief get the name of this component. immutable const char*.
        const char* GetName() { return "CPU Generic Base Class (error)"; };
    };
}