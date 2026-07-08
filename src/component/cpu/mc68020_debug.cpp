#include <component/cpu/mc68020.hpp>

namespace Iris
{
    char* MC68020DebuggerSystem::DisasmInstruction(size_t start)
    {
        nextInstructionSize = moiraCpu->disassemble(disasmBuf, (uint32_t)start);
        return disasmBuf;
    }

    size_t MC68020DebuggerSystem::GetPC()
    {
        return moiraCpu->getPC();
    }    
}