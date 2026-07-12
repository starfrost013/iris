
#include <imgui.h>
#include <component/ip2/ip2_mmu.hpp>

namespace Iris
{
    void CoherentExtensionIP2MMU::AddUI()
    {
        MMU_IP2* mmu = (MMU_IP2*)component;

        ImGui::SetNextWindowSize(ImVec2(700, 200));

        if (!ImGui::Begin("MMU State", &enabled))
            goto end;

        ImGui::Text("OS Base [0x%x]:          0x%x", REG_OS_BASE, mmu->osBase);
        ImGui::Text("Status [0x%x]:           0x%x", REG_STATUS, mmu->status);
        ImGui::Text("Parity [0x%x]:           0x%x", REG_PARITY, mmu->parity);
        ImGui::Text("Multibus Protect [0x%x]: 0x%x", REG_MULTIBUS_PROTECT, mmu->multibusProtect);
        ImGui::Text("Text/Data Base [0x%x]:   0x%x", REG_TEXTDATA_BASE, mmu->textdataBase);
        ImGui::Text("Text/Data Limit [0x%x]   0x%x", REG_TEXTDATA_LIMIT, mmu->textdataLimit);
        ImGui::Text("Stack Base [0x%x]:       0x%x", REG_STACK_BASE, mmu->stackBase);
        ImGui::Text("Stack Limit [0x%x]:      0x%x", REG_STACK_LIMIT, mmu->stackLimit);

        ImGui::Button("View Pagetable");
        
    end:
        ImGui::End();
    }
}