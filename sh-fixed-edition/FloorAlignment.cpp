#include "pch.h"
#include "FloorAlignment.h"
#include "MemAccess.h"
#include <cstring>

#if defined(_M_IX86)
namespace
{
    const uintptr_t originalAlignment = 0x5937B0;

    void __cdecl AlignPathFloor(void* player, float* normal, float* angles)
    {
        int32_t previous[3];
        std::memcpy(previous, static_cast<char*>(player) + 0x120, sizeof(previous));
        ConsoleFloorAlignment(previous, normal, angles);
    }

    // The two LevelCollision callers pass player in EBP, normal in ECX,
    // and output angles on the stack. Other movement retains the PC helper.
    __declspec(naked) void FloorAlignmentHook()
    {
        __asm
        {
            pushfd
            test dword ptr [ebp + 1BCh], 2000h
            jz original
            pushad
            mov ebx, esp
            sub esp, 528
            and esp, -16
            fxsave [esp]
            fninit
            fldcw word ptr [esp]
            push dword ptr [ebx + 40]
            push ecx
            push ebp
            call AlignPathFloor
            add esp, 12
            fxrstor [esp]
            mov esp, ebx
            popad
            popfd
            ret
        original:
            popfd
            jmp dword ptr [originalAlignment]
        }
    }
}
#endif

void RestorePathFloorAlignment()
{
#if defined(_M_IX86)
    static bool installed = false;
    if (installed) return;

    // Refuse changed call sites, including another mod already hooking them.
    const unsigned char first[] = { 0xE8, 0x95, 0xB3, 0xFF, 0xFF };
    const unsigned char second[] = { 0xE8, 0x59, 0xAD, 0xFF, 0xFF };
    if (std::memcmp(reinterpret_cast<void*>(0x598416), first, sizeof(first)) ||
        std::memcmp(reinterpret_cast<void*>(0x598A52), second, sizeof(second)))
        return;

    WriteCall(reinterpret_cast<void*>(0x598416), reinterpret_cast<void*>(FloorAlignmentHook));
    WriteCall(reinterpret_cast<void*>(0x598A52), reinterpret_cast<void*>(FloorAlignmentHook));
    FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<void*>(0x598416), 5);
    FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<void*>(0x598A52), 5);
    installed = true;
#endif
}
