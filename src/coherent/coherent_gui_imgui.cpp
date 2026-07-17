#include <coherent/coherent.hpp>
#include <coherent/coherent_gui_imgui.hpp>

namespace Iris
{
    void CoherentUI::LeftAlign()
    {
        auto targetCursorX = ImGui::GetWindowSize().x - ImGui::GetContentRegionAvail().x;
        ImGui::SetCursorPosX(targetCursorX);
    }

    void CoherentUI::CentreAlign(ImVec2 contentSize)
    {
        auto targetCursorX = ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x / 2) - (contentSize.x / 2);
        ImGui::SetCursorPosX(targetCursorX);
    }

    void CoherentUI::RightAlign(ImVec2 contentSize)
    {
        auto targetCursorX = ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - contentSize.x;
        ImGui::SetCursorPosX(targetCursorX);
    }

    /// @brief draw one of the right-pane guard windows of the Coherent debugger
    /// @param windowType the type of the subwindow to draw 
    /// @param size the size of the window to draw
    void CoherentUI::DrawGuardWindow(CoherentUI::GuardWindowType windowType, ImVec2 size)
    {
        const char* headerText = "";
        // buf used for inserting the guard's addrss
        char* addrBuf = addrBufForWatchpoints;


        // so this is what lambdas are used for
        auto processItem = [](const auto& pair, GuardWindowType windowType, int32_t index)
        {
            const Coherent::Guard& guard = pair.second;

            if (!guard.enabled) // bp is disabled
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
            else if (guard.active) // bp is active
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.6f, 0.2f, 1.0f));
            else // bp is enabled but not hit
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

            if (windowType == GuardWindowBreakpoint)
                ImGui::Text("[%d] break at 0x%lx", index, guard.addr);
            else if (windowType == GuardWindowWatchpoint)
            {
                Coherent::Watchpoint& watchpoint = (Coherent::Watchpoint&)pair.second;
                ImGui::Text("[%d] addr [%lx] = %x", index, guard.addr, watchpoint.GetValue());
            }
            else if (windowType == GuardWindowCatchpoint)
            {
                // should be fine
                const Coherent::Catchpoint& catchpoint = (Coherent::Catchpoint&)pair.second;
                ImGui::Text("[%d] catch on exception #%ld from 0x%lx", index, catchpoint.exceptionId, guard.addr);
            }

            ImGui::PopStyleColor();
        };

        switch (windowType)
        {
            case GuardWindowBreakpoint:
                headerText = "Breakpoints";
                addrBuf = addrBufForBreakpoints;
                break;
            case GuardWindowCatchpoint:
                headerText = "Catchpoints";
                addrBuf = addrBufForCatchpoints;
                break;
            case GuardWindowWatchpoint:
                headerText = "Watchpoints";
                addrBuf = addrBufForWatchpoints;
                break;
        }       

        // draw the "top" of the window
        if (ImGui::BeginChild(headerText, size))
        {                
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.8f, 1.0f, 1.0f));
            ImGui::Text(headerText);
            ImGui::PopStyleColor();

            // if the user clicked the add button add the guard type
            if (ImGui::Button("Add"))
            {
                auto addr = (size_t)strtol(addrBuf, NULL, 16);

                if (windowType == GuardWindowBreakpoint)
                {
                    Coherent::Breakpoint breakpoint = Coherent::Breakpoint(addr);
                    Coherent::AddBreakpoint(breakpoint);
                }
                else if (windowType == GuardWindowWatchpoint)
                {
                    Coherent::Watchpoint watchpoint = Coherent::Watchpoint(addr);
                    Coherent::AddWatchpoint(watchpoint);
                }
                else if (windowType == GuardWindowCatchpoint)
                {
                    Coherent::Catchpoint catchpoint = Coherent::Catchpoint(addr, 0);
                    Coherent::AddCatchpoint(catchpoint);
                }
                else
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "***** INVALID guard window type *****");
            }

            ImGui::SameLine();
            ImGui::InputText("Addr: ", addrBuf, STRING_MAX_LONG, ImGuiInputTextFlags_CharsHexadecimal);
        
            int32_t index = 0;

            switch (windowType)
            {
                case GuardWindowBreakpoint:
                    for (const auto& guard : Coherent::breakpoints) processItem(guard, windowType, index++);
                    break;
                case GuardWindowCatchpoint:
                    for (const auto& guard : Coherent::catchpoints) processItem(guard, windowType, index++);
                    break;
                case GuardWindowWatchpoint:
                    for (const auto& guard : Coherent::watchpoints) processItem(guard, windowType, index++);
                    break;
            }   

            ImGui::Button("Remove");

            ImGui::EndChild();
        }
        
    }
}