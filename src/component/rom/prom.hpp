#include <Iris.hpp>
#include <component/addrspace.hpp>

namespace Iris
{
    extern Cvar* promPath;
    extern Cvar* promSize;

    class PROM : public Component
    {

    public: 
    
        void Start() override;
        void Shutdown() override;

        const char* GetName() { return "IRIS 3130 System PROM"; };

    private: 
        uint8_t* rom;
    };
}