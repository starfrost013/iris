/*
    C    O    H    E    R    E    N    T
    Extensible Emulator Debugging Tools!

    Coherent is an extensible debugger for emulators that is intended to allow the debugging of multiple types of CPU cores in an easy way.
*/

// coherent_gui.cpp: Coherent Imgui Implementation

// not sure where else to put this. I didn't want it to be accessible from outside of UI parts, but i don't feel like a UI backend is a good use of time rn

#include <imgui.h>
#include <base/emulation.hpp>
#include <coherent/coherent.hpp>

namespace Iris
{
    void Coherent::DrawMainWindow()
    {
        ImGui::SetNextWindowPos(ImVec2(
        (Emulation::GetRenderer()->GetWindowSizeX() / 2) - 800,
        (Emulation::GetRenderer()->GetWindowSizeY() / 2) - 600), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(800, 600));

        if (!ImGui::Begin(COHERENT_VERSION, &Coherent::active, ImGuiWindowFlags_MenuBar))
            ImGui::End();
        else 
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Peripherals"))
                {
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }


            // main window text
            ImGui::Text("System Debugger");

            ImGui::Button("Start CPU");
            ImGui::Button("Stop CPU");
            ImGui::Button("Single Step");
            ImGui::Button("Clock Speed");
            ImGui::End();
        }
    }
    
    void Coherent::Frame()
    {
        DrawMainWindow();
        DrawLogWindow();
    }

}
