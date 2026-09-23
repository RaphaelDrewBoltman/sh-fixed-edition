#include "pch.h"
#include "ChaotixSpotlightColors.h"
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstring>

#if defined(_M_IX86)
#include "renderware.h"

namespace
{
    constexpr uintptr_t imageBase = 0x400000;
    constexpr uintptr_t coneDrawCall = 0x588f84;
    constexpr uintptr_t colorTable = 0x8dd90c;

    static_assert(offsetof(RpAtomic, geometry) == 0x18, "Unexpected RpAtomic geometry offset");

    using RenderClump = RpClump* (__cdecl*)(RpClump*);
    RenderClump chainedRender = nullptr;

    RpAtomic* __cdecl RefreshConeGeometry(RpAtomic* atomic, void*)
    {
        if (atomic && atomic->geometry)
        {
            // The three cones share geometry. Invalidating its PC RenderWare
            // instance before each draw applies that cone's material color.
            if (RpGeometryLock(atomic->geometry, rpGEOMETRYLOCKVERTICES))
                RpGeometryUnlock(atomic->geometry);
        }
        return atomic;
    }

    RpClump* __cdecl RenderSpotlightCone(RpClump* clump)
    {
        if (clump)
            RpClumpForAllAtomics(clump, RefreshConeGeometry, nullptr);
        return chainedRender(clump);
    }

    bool WriteCall(uintptr_t address, void* target)
    {
        const auto distance = static_cast<int64_t>(reinterpret_cast<uintptr_t>(target)) - static_cast<int64_t>(address + 5);
        if (distance < INT32_MIN || distance > INT32_MAX)
            return false;

        const auto displacement = static_cast<int32_t>(distance);
        uint8_t patch[5] = { 0xe8 };
        std::memcpy(patch + 1, &displacement, sizeof(displacement));

        DWORD oldProtection;
        if (!VirtualProtect(reinterpret_cast<void*>(address), sizeof(patch), PAGE_EXECUTE_READWRITE, &oldProtection))
            return false;

        std::memcpy(reinterpret_cast<void*>(address), patch, sizeof(patch));
        FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<void*>(address), sizeof(patch));

        DWORD ignored;
        return VirtualProtect(reinterpret_cast<void*>(address), sizeof(patch), oldProtection, &ignored) != 0;
    }
}
#endif

void RestoreChaotixSpotlightColors()
{
#if defined(_M_IX86)
    static bool installed = false;
    if (installed || reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr)) != imageBase)
        return;

    // Validate the known PC build before patching, and preserve a preceding hook.
    constexpr uint8_t colors[] = {
        0x80, 0x00, 0xff, 0x40, // purple
        0xff, 0x80, 0x00, 0x40, // orange
        0x00, 0xff, 0x00, 0x40  // green
    };
    constexpr uint8_t precedingCode[] = { 0x8b, 0x56, 0x60, 0x89, 0x51, 0x04, 0x50 };
    if (std::memcmp(reinterpret_cast<void*>(colorTable), colors, sizeof(colors)) != 0 ||
        std::memcmp(reinterpret_cast<void*>(coneDrawCall - sizeof(precedingCode)), precedingCode, sizeof(precedingCode)) != 0 ||
        *reinterpret_cast<uint8_t*>(coneDrawCall) != 0xe8)
        return;

    const auto displacement = *reinterpret_cast<int32_t*>(coneDrawCall + 1);
    chainedRender = reinterpret_cast<RenderClump>(coneDrawCall + 5 + displacement);
    if (reinterpret_cast<void*>(chainedRender) == reinterpret_cast<void*>(&RenderSpotlightCone))
        return;
    installed = WriteCall(coneDrawCall, reinterpret_cast<void*>(&RenderSpotlightCone));
#endif
}
