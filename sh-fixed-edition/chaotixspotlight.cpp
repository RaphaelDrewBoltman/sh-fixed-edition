#include "pch.h"
#include "renderware.h"
#include "helper.h"

static RpClump* __cdecl SpotlightClumpRender(RpClump* clump)
{
    RpClumpForAllAtomics(clump, objRpAtomicSetGeometryFlagToModulateMaterialColor, nullptr);
    return RpClumpRender(clump);
}

void ChaotixSpotlight()
{
    WriteCall((void*)0x588f84, SpotlightClumpRender);
}