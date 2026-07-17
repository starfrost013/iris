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
    /// @brief a basic about window since coherent is retargetable
    void CoherentUI::DrawAboutWindow()
    {
        if (ImGui::Begin("About", &CoherentUI::aboutActive))
        {
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), COHERENT_VERSION);
            ImGui::Text("An architecture-agnostic, retargetable, embedded debugger for emulators");
            ImGui::Text("© 2026 starfrost (https://starfrost.net/)");

            if (ImGui::Button("Close"))
                CoherentUI::aboutActive = false; 

            ImGui::End();
        }
    }

    void CoherentUI::DrawMainWindow()
    {
        ImGui::SetNextWindowPos(ImVec2(
        (Emulation::GetRenderer()->GetWindowSizeX() / 2) - 800,
        (Emulation::GetRenderer()->GetWindowSizeY() / 2) - 600), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(800, 600));

        int i = 0, pcOffset = 0;
        ImVec2 topBarSize = ImVec2(800, 25);
        
        ImVec2 registerPaneSize = ImVec2(180, 600);
        ImVec2 disasmPaneSize = ImVec2(340, 600);
        ImVec2 debugContainerPaneSize = ImVec2(290, 600);
        ImVec2 debugContainerChildWindowSize = ImVec2(290, 170);

        // create the UI windows for any extensions if they exist
        for (CoherentExtension* extension : Coherent::extensions)
        {
            if (extension->enabled)
                extension->AddUI(); 
        }

        if (ImGui::Begin("Coherent Debugger", &Coherent::active, ImGuiWindowFlags_MenuBar))
        {
            if (ImGui::BeginMenuBar())
            {
                // Extensions

                if (ImGui::BeginMenu("Peripherals"))
                {
                    // see which extensions are enabled 

                    for (CoherentExtension* extension : Coherent::extensions)
                    {
                        if (ImGui::MenuItem(extension->component->GetName()))
                            extension->enabled = true; 
                    }

                    ImGui::EndMenu();
                }

                // Help / about window 

                if (ImGui::BeginMenu("Help"))
                {
                    if (ImGui::MenuItem("About"))
                        CoherentUI::aboutActive = true;

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            // top pane

            if (ImGui::BeginChild("DebugViewPane", topBarSize))
            {
                if (Coherent::currentSystem == nullptr)
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 0.0f), "***** Error - No System Active *****");
                else
                {
                    if (Coherent::currentSystem->GetRunState() == CoherentSystem::RunState::Running)
                    {
                        if (ImGui::Button("Pause CPU"))
                            Coherent::currentSystem->SetRunState(CoherentSystem::RunState::Paused);
                    }
                    else
                    {
                        if (ImGui::Button("Start CPU"))
                            Coherent::currentSystem->SetRunState(CoherentSystem::RunState::Running); 
                    }

                    ImGui::SameLine();

                    if (ImGui::Button("Reset"))
                        Coherent::currentSystem->SetRunState(CoherentSystem::RunState::Reset);

                    if (Coherent::currentSystem->GetRunState() == CoherentSystem::RunState::Paused)
                    {
                        ImGui::SameLine();
                        
                        if (ImGui::Button("Step"))
                            Coherent::currentSystem->SetRunState(CoherentSystem::RunState::SingleStep);
                    }

                    ImGui::SameLine();
                    ImGui::Text("Clock Speed: %.2f MHz", ((float)Emulation::GetMachine().FindComponentByType<ComponentCPU>()->GetClockSpeed()) / 1000000.0);
                }
                
                ImGui::EndChild();
            }  

            // "left" (register) pane
            if (ImGui::BeginChild("RegisterPane", registerPaneSize))
            {
                for (auto aRegister : Coherent::currentSystem->registers)
                {
                    auto value = aRegister.second->Read();

                    // evil
                    if (value.type() == typeid(uint8_t)
                    || value.type() == typeid(int8_t))
                    {
                        uint8_t formattedValue = std::any_cast<uint8_t>(value);
                        ImGui::Text("%s: %02x", aRegister.first, formattedValue);
                    }
                    else if (value.type() == typeid(uint16_t)
                    || value.type() == typeid(int16_t))
                    {
                        uint16_t formattedValue = std::any_cast<uint16_t>(value);
                        ImGui::Text("%s: %04x", aRegister.first, formattedValue);
                    }
                    else if (value.type() == typeid(uint32_t)
                    || value.type() == typeid(int32_t))
                    {
                        uint32_t formattedValue = std::any_cast<uint32_t>(value);
                        ImGui::Text("%s: %08x", aRegister.first, formattedValue);
                    }
                    
                    i++;
                }

                ImGui::EndChild();
            }

            ImGui::SameLine();

            // middle pane; disassembly pane
            if (ImGui::BeginChild("DisassemblyPane", disasmPaneSize))
            {
                // todo: MUST put in a buffer...
                for (i = 0; i < 30; i++)
                {
                    if (i == 0)
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.6f, 0.9f, 1.0f));

                    auto addr = Coherent::currentSystem->GetPC() + pcOffset;

                    ImGui::Text("0x%lx:    %s", addr, Coherent::currentSystem->DisasmInstruction(addr));

                    if (i == 0)
                        ImGui::PopStyleColor();

                    pcOffset += Coherent::currentSystem->GetNextInstructionSize();
                }

                ImGui::EndChild();
            }

            ImGui::SameLine();

            // right pane: debug controls
            if (ImGui::BeginChild("DebugControlsPane", debugContainerPaneSize))
            {
                CoherentUI::DrawGuardWindow(CoherentUI::GuardWindowType::GuardWindowBreakpoint, debugContainerChildWindowSize);
                CoherentUI::DrawGuardWindow(CoherentUI::GuardWindowType::GuardWindowWatchpoint, debugContainerChildWindowSize);
                CoherentUI::DrawGuardWindow(CoherentUI::GuardWindowType::GuardWindowCatchpoint, debugContainerChildWindowSize);

                ImGui::EndChild();
            }
          
            ImGui::End();
        }
    }
    
    void Coherent::Frame()
    {
        CoherentUI::DrawMainWindow();
        CoherentUI::DrawLogWindow();

        if (CoherentUI::aboutActive)
            CoherentUI::DrawAboutWindow();
    }
}
