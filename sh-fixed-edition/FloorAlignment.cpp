#include "pch.h"
#include "MemAccess.h"
#include <cmath>
#include "helper.h"
#include "FloorAlignment.h"

const uintptr_t originalAlignmentFunc = 0x5937B0;

// Retail GC 800EC828. The PC helper 5937B0 instead projects the old heading
// onto the new floor and rewrites yaw. On a twisting path that removes the
// sideways velocity on which the ordinary movement friction acts.
void __cdecl AlignPathFloor(void* player, RwV3d* normal, float* angles)
{
	sAngle previous;
	std::memcpy(&previous, static_cast<char*>(player) + 0x120, sizeof(previous));

	Angle pitch, roll, yaw = NJM_DEG_ANG(angles[1]);
	const float x = normal->x, y = normal->y, z = normal->z;

	if (z > 0.9658f || (z > 0.95f && std::abs(y) < 0.0009f))
	{
		pitch = 0x4000;
		roll = previous.z;
		normal->x = 0.0f;
		normal->y = 0.0f;
		normal->z = 1.0f;
	}
	else if (z < -0.9658f || (z < -0.95f && std::abs(y) < 0.0009f))
	{
		pitch = 0xC000;
		roll = previous.z;
		normal->x = 0.0f;
		normal->y = 0.0f;
		normal->z = -1.0f;
	}
	else
	{
		if (y > 0.99608f)
		{
			pitch = 0;
			roll = 0;
			normal->x = 0.0f;
			normal->z = 0.0f;
			normal->y = 1.0f;
		}
		else if (y < -0.99608f)
		{
			pitch = 0x8000;
			roll = 0;
			normal->x = 0.0f;
			normal->z = 0.0f;
			normal->y = -1.0f;
		}
		else
		{
			pitch = NJM_RAD_ANG(std::asin(z));
			roll = -NJM_RAD_ANG(std::atan2(x, y));
		}

		if (std::abs(static_cast<int>(static_cast<int16_t>(previous.z - roll))) > 0x4000)
		{
			roll += 0x8000; pitch = 0x8000 - pitch;
		}

		if ((previous.x == 0x4000 || previous.x == 0xC000) && pitch != 0x4000 && pitch != 0xC000)
		{
			// PC may revisit this transition with already-adjusted output angles.
			// Anchor to the saved orientation so collision retries do not accumulate yaw.
			yaw = previous.z + previous.y - roll;
		}
	}

	angles[0] = NJM_ANG_DEG(pitch);
	angles[1] = NJM_ANG_DEG(yaw);
	angles[2] = NJM_ANG_DEG(roll);
}

// The two LevelCollision callers pass player in EBP, normal in ECX,
// and output angles on the stack. Other movement retains the PC helper.
__declspec(naked) void AlignPathFloorHook()
{
	__asm
	{
		pushfd
		test dword ptr[ebp + 1BCh], 2000h
		jz original
		pushad
		mov ebx, esp
		sub esp, 528
		and esp, -16
		fxsave[esp]
		fninit
		fldcw word ptr[esp]
		push dword ptr[ebx + 40]
		push ecx
		push ebp
		call AlignPathFloor
		add esp, 12
		fxrstor[esp]
		mov esp, ebx
		popad
		popfd
		ret
		original :
		popfd
		jmp dword ptr[originalAlignmentFunc]
	}
}

void RestorePathFloorAlignment()
{
	WriteCall(reinterpret_cast<void*>(0x598416), reinterpret_cast<void*>(AlignPathFloorHook));
	WriteCall(reinterpret_cast<void*>(0x598A52), reinterpret_cast<void*>(AlignPathFloorHook));
}
