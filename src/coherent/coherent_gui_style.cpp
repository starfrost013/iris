
/*
    C    O    H    E    R    E    N    T
    Extensible Emulator Debugging Tools!

    Coherent is an extensible debugger for emulators that is intended to allow the debugging of multiple types of CPU cores in an easy way.

    coherent_gui_style.cpp: Implement the Coherent Style system for IMGUI
*/

#include <coherent/coherent.hpp>
#include <coherent/coherent_gui_imgui.hpp>

namespace Iris
{
    void CoherentUI::InitStyleDefault(UIStyle style, ImGuiStyle& styleObj)
    {
        // we make one change to default imgui style -
        // there is no colour specified for selectables when they are selected...
        styleObj.Colors[ImGuiCol_Header] = ImVec4(0.3, 0.6, 0.7, 1.0);
    }

    void CoherentUI::InitStyleMex(UIStyle style, ImGuiStyle& styleObj)
    {
    }

    void CoherentUI::InitStyle(UIStyle style)
    {
        ImGuiStyle& styleObj = ImGui::GetStyle();

        switch (style)
        {
            case UIStyle::Default:
                break;
            case UIStyle::MEX:
                InitStyleMex(style, styleObj);
                break;
        }
    }
  
}