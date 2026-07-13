/*
    C    O    H    E    R    E    N    T
    Extensible Emulator Debugging Tools!

    Coherent is an extensible debugger for emulators that is intended to allow the debugging of multiple types of CPU cores in an easy way.
*/

// coherent_gui.cpp: Coherent Imgui Implementation

// not sure where else to put this. I didn't want it to be accessible from outside of UI parts, but i don't feel like a UI backend is a good use of time rn

#include <coherent/coherent.hpp>
#include <coherent/coherent_gui_imgui.hpp>
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
        int pcOffset = 0;

        for (CoherentExtension* extension : extensions)
        {
            if (extension->enabled)
                extension->AddUI(); 
        }

        if (!ImGui::Begin(COHERENT_VERSION, &active, ImGuiWindowFlags_MenuBar))
            goto end;
        else 
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Peripherals"))
                {
                    // see which extensions are enabled 

                    for (CoherentExtension* extension : extensions)
                    {
                        if (ImGui::MenuItem(extension->component->GetName()))
                            extension->enabled = true; 
                    }

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

            if (currentSystem->GetRunState() == CoherentSystem::RunState::Running)
            {
                if (ImGui::Button("Pause CPU"))
                    currentSystem->SetRunState(CoherentSystem::RunState::Paused);
            }
            else
            {
                if (ImGui::Button("Start CPU"))
                    currentSystem->SetRunState(CoherentSystem::RunState::Running); 
            }

            ImGui::SameLine();
            if (ImGui::Button("Reset"))
                currentSystem->SetRunState(CoherentSystem::RunState::Reset);

            if (currentSystem->GetRunState() == CoherentSystem::RunState::Paused)
            {
                ImGui::SameLine();
                
                if (ImGui::Button("Step"))
                    currentSystem->SetRunState(CoherentSystem::RunState::SingleStep);
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

                if ((i % 5) != 4)
                    ImGui::SameLine();
                
                i++;
            }

            ImGui::NewLine();

            // todo: MUST put in a buffer...
            for (i = 0; i < 30; i++)
            {
                if (i == 0)
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.6f, 0.9f, 1.0f));

                auto addr = currentSystem->GetPC() + pcOffset;

                ImGui::Text("0x%lx:    %s", addr, currentSystem->DisasmInstruction(addr));

                if (i == 0)
                    ImGui::PopStyleColor();

                pcOffset += currentSystem->GetNextInstructionSize();
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
