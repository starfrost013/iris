#include <base/machine/machine.hpp>
#include <coherent/coherent.hpp>

namespace Iris
{
    void Machine::Start()
    {
        for (Component* component : components)
            component->Start();
    }

    void Machine::Tick()
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
                    goto run;
                }
                
            }
            else
                goto run;
                
        run:
            component->Tick();
        }
    }


    void Machine::Shutdown()
    {
        for (Component* component : components)
        {
            component->Shutdown();
            components.pop_back();
            delete component;
        }
    }
}