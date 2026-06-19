/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost
*/

//
// util.hpp: Various misc. utilities
//

#include <Iris.hpp>

class Util
{
    #define Chrono_GetTime() std::chrono::steady_clock::now();
    #define Chrono_GetTicksMS(start, end) std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    #define Chrono_GetTicksUS(start, end) std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    #define Chrono_GetTicksNS(start, end) std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

};