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

        int i = 0;

        if (!ImGui::Begin(COHERENT_VERSION, &Coherent::active, ImGuiWindowFlags_MenuBar))
            goto end;
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
            if (Coherent::currentSystem == nullptr)
            {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 0.0f), "***** Error - No System Active *****");
                goto end;
            }
            else 
                ImGui::Text("Debug Controls: ");
            ImGui::SameLine();
            ImGui::Button("Start CPU");
            ImGui::SameLine();
            ImGui::Button("Stop CPU");
            ImGui::SameLine();
            ImGui::Button("Single Step");
            ImGui::SameLine();
            ImGui::Button("Clock Speed");
            ImGui::SameLine();

            ImGui::NewLine();

            for (auto aRegister : Coherent::currentSystem->registers)
            {

                ImGui::Text(aRegister.first);
                ImGui::SameLine();

                auto value = aRegister.second->Read();

                // evil
                if (value.type() == typeid(uint8_t)
                || value.type() == typeid(int8_t))
                {
                    uint8_t formattedValue = std::any_cast<uint8_t>(value);
                    ImGui::Text(": %02x", formattedValue);
                }
                else if (value.type() == typeid(uint16_t)
                || value.type() == typeid(int16_t))
                {
                    uint16_t formattedValue = std::any_cast<uint16_t>(value);
                    ImGui::Text(": %04x", formattedValue);
                }
                else if (value.type() == typeid(uint32_t)
                || value.type() == typeid(int32_t))
                {
                    uint32_t formattedValue = std::any_cast<uint32_t>(value);
                    ImGui::Text(": %08x", formattedValue);
                }

                if ((i % 4) != 3)
                    ImGui::SameLine();
                
                i++;
            }

        end:
            ImGui::End();
        }
    }
    
    void Coherent::Frame()
    {
        DrawMainWindow();
        DrawLogWindow();
    }

}
