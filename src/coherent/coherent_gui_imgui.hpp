
/*
    C    O    H    E    R    E    N    T
    Extensible Emulator Debugging Tools!

    Coherent is an extensible debugger for emulators that is intended to allow the debugging of multiple types of CPU cores in an easy way.

    coherent_gui_imgui.hpp: Provides all includes for IMGUI type of Coherent GUI & utilities for IMGUI
*/

#pragma once

#include <imgui.h>

namespace Iris 
{
    class ImGuiUtils
    {
    public:
        static void LeftAlign();
        static void CentreAlign(ImVec2 contentSize);
        static void RightAlign(ImVec2 contentSize);
    };
}