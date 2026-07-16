#include <coherent/coherent.hpp>
#include <coherent/coherent_gui_imgui.hpp>

namespace Iris
{
    void ImGuiUtils::LeftAlign()
    {
        auto targetCursorX = ImGui::GetWindowSize().x - ImGui::GetContentRegionAvail().x;
        ImGui::SetCursorPosX(targetCursorX);
    }

    void ImGuiUtils::CentreAlign(ImVec2 contentSize)
    {
        auto targetCursorX = ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x / 2) - (contentSize.x / 2);
        ImGui::SetCursorPosX(targetCursorX);
    }

    void ImGuiUtils::RightAlign(ImVec2 contentSize)
    {
        auto targetCursorX = ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - contentSize.x;
        ImGui::SetCursorPosX(targetCursorX);
    }

}