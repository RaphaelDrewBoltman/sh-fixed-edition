#include "pch.h"
#include <stdint.h>
#include "MemAccess.h"
#include "helper.h"
#include "renderware.h"

// The game does not fix the endian of ptclplay files, preventing some particle effects to play

#define SWAP16(a) \
    a = (uint16_t)( \
        (((uint16_t)(a) & 0x00FFu) << 8) | \
        (((uint16_t)(a) & 0xFF00u) >> 8))

#define SWAP32(a) \
    a = (uint32_t)( \
        (((uint32_t)(a) & 0x000000FFu) << 24) | \
        (((uint32_t)(a) & 0x0000FF00u) << 8)  | \
        (((uint32_t)(a) & 0x00FF0000u) >> 8)  | \
        (((uint32_t)(a) & 0xFF000000u) >> 24) )

#define SWAPF32(a) do { \
        union { float f; uint32_t u; } _v; \
        _v.f = (a); \
        _v.u = SWAP32(_v.u); \
        a = _v.f; \
    } while(0)

RwInt32 __cdecl LoadFile_PtclPlay(RwChar* fname, void* buff)
{
    RwInt32 result = LoadFile(fname, buff);

    if (result > 0)
    {
        PtclPlayData* data = (PtclPlayData*)buff;
        for (int i = 0; i < result / sizeof(PtclPlayData); ++i) // Always 16x32 entries
        {
            PtclPlayData* current = &data[i];
            SWAP16(current->timer);
            SWAP32(current->ang.x);
            SWAP32(current->ang.y);
            SWAP32(current->ang.z);
            SWAPF32(current->pos.x);
            SWAPF32(current->pos.y);
            SWAPF32(current->pos.z);
            SWAPF32(current->vec.x);
            SWAPF32(current->vec.y);
            SWAPF32(current->vec.z);
        }
    }

    return result;
}

static void __declspec(naked) LoadFile_PtclPlay_w()
{
    __asm
    {
        push[esp + 04h] // buff
        push eax // fname
        call LoadFile_PtclPlay
        add esp, 8
        retn
    }
}

void PtclPlay()
{
    WriteCall((void*)0x42FADC, LoadFile_PtclPlay_w);
}