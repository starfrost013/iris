
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
        styleObj.Colors[ImGuiCol_Header] = ImVec4(0.1, 0.2, 0.7, 1.0);
        styleObj.Colors[ImGuiCol_HeaderActive] = ImVec4(0.1, 0.2, 0.5, 1.0);
        styleObj.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1, 0.2, 0.6, 1.0);
    }

    void CoherentUI::InitStyleExperimental(UIStyle style, ImGuiStyle& styleObj)
    {
        styleObj.Colors[ImGuiCol_TitleBg] = ImVec4(0.8, 0.4, 0.2, 0.75);
        styleObj.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0, 0.3, 0.4, 0.75);
        styleObj.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.6, 0.2, 0.1, 0.75);
        styleObj.Colors[ImGuiCol_Border] = ImVec4(1.0, 1.0, 1.0, 1.0);
        styleObj.Colors[ImGuiCol_Button] = ImVec4(0.1, 0.45, 0.1, 1.0);
        styleObj.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.05, 0.35, 0.05, 1.0);
        styleObj.Colors[ImGuiCol_ButtonActive] = ImVec4(0.0, 0.25, 0.0, 1.0);
        styleObj.WindowTitleAlign = ImVec2(0.5, 0.5);
        styleObj.WindowRounding = 8;    
    }

    void CoherentUI::InitStyleMex(UIStyle style, ImGuiStyle& styleObj)
    {
        styleObj.Colors[ImGuiCol_TitleBg] = ImVec4(0.8, 0.8, 0.8, 1.0);
        styleObj.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0, 1.0, 1.0, 1.0);
        styleObj.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.9, 0.9, 0.9, 1.0);
        // from a crappy screenshot of mex since it doesn't run yet in the emulator
        // note ; really copies the title bar style rather than the style of MEX itself.
        styleObj.Colors[ImGuiCol_Text] = ImVec4(0.194, 0.273, 0.802, 1.0);
        styleObj.Colors[ImGuiCol_Header] = ImVec4(0.094, 0.173, 0.902, 1.0);
        styleObj.Colors[ImGuiCol_WindowBg] = ImVec4(0.0, 0.0, 0.0, 1.0);
        styleObj.Colors[ImGuiCol_Border] = ImVec4(1.0, 1.0, 1.0, 1.0);
        styleObj.WindowBorderSize = 2;
        styleObj.WindowRounding = 0;
    }

    void CoherentUI::InitStyle(UIStyle style)
    {
        ImGuiStyle& styleObj = ImGui::GetStyle();

        // reset the style bu tdon't do it if we have NOT already set it 

        if (alreadySetStyle)
        {
            ImGui::GetStyle() = ImGuiStyle(); 
            ImGui::StyleColorsDark();
        }

        alreadySetStyle = true; 
        
        switch (style)
        {
            case UIStyle::Default:
                InitStyleDefault(style, styleObj);
                break;
            case UIStyle::MEX:
                InitStyleMex(style, styleObj);
                break;
            case UIStyle::Experimental:
                InitStyleExperimental(style, styleObj);
                break;
        }
    }
  
}