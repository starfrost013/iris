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
#include <component/cpu/cpu.hpp>

namespace Iris
{
    void Coherent::DrawMainWindow()
    {
        ImGui::SetNextWindowPos(ImVec2(
        (Emulation::GetRenderer()->GetWindowSizeX() / 2) - 800,
        (Emulation::GetRenderer()->GetWindowSizeY() / 2) - 600), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(800, 600));

        int i = 0;

        if (!ImGui::Begin(COHERENT_VERSION, &active, ImGuiWindowFlags_MenuBar))
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

            if (currentSystem->runState == CoherentSystem::RunState::Running)
            {
                if (ImGui::Button("Pause CPU"))
                    currentSystem->runState = CoherentSystem::RunState::Paused;
            }
            else
            {
                if (ImGui::Button("Start CPU"))
                    currentSystem->runState = CoherentSystem::RunState::Running; 
            }

            ImGui::SameLine();
            if (ImGui::Button("Reset"))
                currentSystem->runState = CoherentSystem::RunState::Reset;

            if (currentSystem->runState == CoherentSystem::RunState::Paused)
            {
                ImGui::SameLine();
                
                if (ImGui::Button("Step"))
                    currentSystem->runState = CoherentSystem::RunState::SingleStep;
            }

            
            ImGui::Text("Clock Speed : %.2f MHz", ((float)Emulation::GetMachine().FindComponentByType<ComponentCPU>()->GetClockSpeed()) / 1000000.0);

            for (auto aRegister : currentSystem->registers)
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
        // basic state machine
        // move to execstart?
        if (currentSystem)
        {
            switch (currentSystem->runState)
            {
                case CoherentSystem::RunState::Reset:
                    Emulation::Reset();
                    currentSystem->runState = CoherentSystem::RunState::Running;
                    break;
                case CoherentSystem::RunState::SingleStep:
                    Emulation::SingleStep();
                    currentSystem->runState = CoherentSystem::RunState::Paused;
                    break;
                case CoherentSystem::RunState::Paused:
                    if (!Emulation::GetPaused())
                        Emulation::SetPaused(true);

                    break;
                case CoherentSystem::RunState::Running:
                    if (Emulation::GetPaused())
                        Emulation::SetPaused(false);
            }
        }


        DrawMainWindow();
        DrawLogWindow();
    }
}
