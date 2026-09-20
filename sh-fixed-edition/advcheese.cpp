#include "pch.h"
#include <unordered_map>
#include "FastFunctionHook.hpp"
#include "renderware.h"
#include "helper.h"
#include "advcheese.h"

// Todo: more cleaning, free assets

FastThiscallHook<void, void*> ADV_PLAYER__PDisp_h(0x4534A0);
FastThiscallHook<void*, void*, void*, int*> ADV_PLAYER__ctor_h(0x452FE0);
FastThiscallHook<void*, void*, int> ADV_PLAYER__dtor_h(0x453330);

template<class T> static T& At(void* p, size_t offset) { return *reinterpret_cast<T*>(static_cast<char*>(p) + offset); }
template<class R, class... A> static R Call(uintptr_t address, A... args) { return reinterpret_cast<R(__cdecl*)(A...)>(address)(args...); }

static RwTexDictionary* textures;
static RtAnimAnimation* animations[4];
static RpClump* clumps[2];
static bool loaded;

static void* __cdecl FindHierarchy(void* frame, void* data)
{
	auto found = static_cast<void**>(data);
	if (!*found) *found = Call<void*>(0x6a8640, frame);
	if (!*found) Call<void*>(0x64c5e0, frame, &FindHierarchy, data);
	return *found ? nullptr : frame;
}

static void* __cdecl BindSkin(void* atomic, void* hierarchy)
{
	Call<void*>(0x6a06b0, atomic, hierarchy);
	return atomic;
}

struct AdvCheese
{
	void* clump = nullptr;
	void* hierarchy = nullptr;
	void* ball = nullptr;
	void* ballAnchor = nullptr;
	int pose = -1;
	uint32_t tick = 0;
	double seconds = 0;

	AdvCheese()
	{
		void* previous = Call<void*>(0x64e450);
		clump = RpClumpClone(clumps[0]);
		if (clump)
			ball = RpClumpClone(clumps[1]);

		Call<void*>(0x64e430, previous);
		if (!clump)
			return;

		FindHierarchy(At<void*>(clump, 4), &hierarchy);
		if (!hierarchy) {
			if (ball)
				Call<int>(0x66bc60, ball);
			Call<int>(0x66bc60, clump);
			return;
		}

		At<unsigned>(hierarchy, 0) |= 0x3000;
		Call<void*>(0x66b540, clump, &BindSkin, hierarchy);
		Call<int>(0x69dfc0, At<void*>(hierarchy, 0x20), animations[0]);
		Call<int>(0x6a8560, hierarchy);

		// Gameplay attaches the separate ball to hierarchy node ID 1.
		auto nodes = static_cast<unsigned char*>(At<void*>(hierarchy, 0x10));
		for (int i = 0; i < At<int>(hierarchy, 4); ++i)
			if (At<int>(nodes + i * 16, 0) == 1) ballAnchor = At<void*>(nodes + i * 16, 12);

		if (ball && !ballAnchor)
		{
			Call<int>(0x66bc60, ball);
			ball = nullptr;
		}

		if (ball)
		{
			Call<void*>(0x66b540, ball, reinterpret_cast<void*>(0x581f70), nullptr);
			Call<void*>(0x669840, *reinterpret_cast<void**>(0x8d69cc), ball);
		}

		// Use the same material/pipeline setup and world lights as the menu characters.
		Call<void*>(0x66b540, clump, reinterpret_cast<void*>(0x581f70), nullptr);
		Call<void*>(0x669840, *reinterpret_cast<void**>(0x8d69cc), clump);
	}

	~AdvCheese()
	{
		if (ball)
		{
			RpWorldRemoveClump(*reinterpret_cast<RpWorld**>(0x8d69cc), (RpClump*)ball);
			RpClumpDestroy((RpClump*)ball);
		}
		if (clump)
		{
			RpWorldRemoveClump(*reinterpret_cast<RpWorld**>(0x8d69cc), (RpClump*)clump);
			RpClumpDestroy((RpClump*)clump);
		}
	}

	AdvCheese(AdvCheese&&) = default;      // move constructor
	AdvCheese(const AdvCheese&) = delete;  // forbid copying
};

static std::unordered_map<void*, AdvCheese*> adv_cheese_map;

static void Load()
{
	if (!loaded)
	{
		auto one = OneFileReader("advertise/adv_pl_cheese.one");

		textures = one.LoadTexDict("CHEESE.TXD");

		const char* names[] = { "CH_CHARANGE_IDLE.ANM","CH_CHARANGE_SELECT.ANM","CH_STORY_IDLE.ANM","CH_STORY_SELECT.ANM" };
		for (int i = 0; i < 4; ++i) animations[i] = one.LoadAnim(names[i]);

		RwTexDictionarySetCurrent(textures);
		clumps[0] = one.LoadClump("CHAO_LOCATOR.DFF");
		clumps[1] = one.LoadClump("CH_KAKU_LOCATOR.DFF");

		loaded = true;
	}
}

static double AdvanceMenuAnimation(double seconds, uint32_t updates, double duration, bool loop)
{
	const double next = seconds + updates / 60.0;
	return loop ? std::fmod(next, duration) : min(next, duration);
}

static void __cdecl RenderCheese(void* player)
{
	if (At<int>(player, 0x190) != 7 || At<int>(player, 0x194) < 3 || !loaded)
		return;

	auto entry = adv_cheese_map.find(player);
	if (entry == adv_cheese_map.end())
		return;

	auto a = entry->second;
	if (!a->clump) return;
	const int index = At<int>(player, 0x40);
	if (index < 0 || index >= 48 || index % 12 != 7) return;
	const int pose = index / 12;
	void* interpolator = At<void*>(a->hierarchy, 0x20);
	const float duration = At<float>(animations[pose], 0xc);
	if (!std::isfinite(duration) || duration <= 0) return;
	const uint32_t tick = At<uint32_t>(player, 0x194);

	if (a->pose != pose)
	{
		Call<int>(0x69dfc0, interpolator, animations[pose]);
		a->pose = pose; a->tick = tick; a->seconds = 0;
	}

	// Count gameplay updates, not renders or Cream's shorter animation cycles.
	a->seconds = AdvanceMenuAnimation(a->seconds, tick - a->tick, duration, pose % 2 == 0);
	a->tick = tick;
	Call<int>(0x69e490, interpolator, static_cast<float>(a->seconds));
	void* root = At<void*>(a->clump, 4);
	void* creamRoot = At<void*>(At<void*>(player, 0xc0), 4);
	memcpy(static_cast<char*>(root) + 0x10, static_cast<char*>(creamRoot) + 0x10, 64);

	// Challenge animations include a translation; story animations are centered
	// at zero and need a separate hovering position beside Cream.
	// In the story camera, negative local X moves right on screen.
	if (pose >= 2)
	{
		float* matrix = reinterpret_cast<float*>(static_cast<char*>(root) + 0x10);
		for (int axis = 0; axis < 3; ++axis)
			matrix[12 + axis] += -3.5f * matrix[axis] - 4.0f * matrix[4 + axis];

		// Move the whole model toward the active camera along its projection rays.
		// Scaling translation and basis together preserves screen position and size.
		void* camera = At<void*>(*reinterpret_cast<void**>(0x8e0a4c), 0);
		if (camera)
		{
			void* cameraFrame = At<void*>(camera, 4);
			const float* cameraMatrix = Call<const float*>(0x64c300, cameraFrame);
			constexpr float depthScale = 0.35f;

			for (int axis = 0; axis < 3; ++axis)
			{
				matrix[12 + axis] = cameraMatrix[12 + axis] + depthScale * (matrix[12 + axis] - cameraMatrix[12 + axis]);
				matrix[axis] *= depthScale;
				matrix[4 + axis] *= depthScale;
				matrix[8 + axis] *= depthScale;
			}

			// The scaled basis is no longer orthonormal or identity.
			reinterpret_cast<unsigned*>(matrix)[3] &= ~0x20003u;
		}
	}

	Call<void*>(0x64c280, root);
	Call<int>(0x6a8660, a->hierarchy);
	Call<void*>(0x66b4f0, a->clump);

	if (a->ball)
	{
		void* ballRoot = At<void*>(a->ball, 4);
		const float* anchor = Call<const float*>(0x64c300, a->ballAnchor);
		float* matrix = reinterpret_cast<float*>(static_cast<char*>(ballRoot) + 0x10);
		memcpy(matrix, anchor, 64);

		// Original gameplay offset (0, 3.41, 0.17), in the animated anchor's space.
		// Its basis also carries the story-menu perspective compensation.
		for (int axis = 0; axis < 3; ++axis)
			matrix[12 + axis] += 3.41f * matrix[4 + axis] + 0.17f * matrix[8 + axis];
		reinterpret_cast<unsigned*>(matrix)[3] &= ~0x20000u;
		Call<void*>(0x64c280, ballRoot);
		Call<void*>(0x66b4f0, a->ball);
	}
}

static void* __fastcall Destroy(void* player, void*, unsigned flags)
{
	auto entry = adv_cheese_map.find(player);
	if (entry != adv_cheese_map.end())
	{
		delete entry->second;
		adv_cheese_map.erase(entry);
	}
	return player;
}

void ADV_PLAYER__PDisp_r(void* _this)
{
	RenderCheese(_this);
	ADV_PLAYER__PDisp_h.Original(_this);
}

void* ADV_PLAYER__ctor_r(void* _this, void* a1, int* a2)
{
	void* result = ADV_PLAYER__ctor_h.Original(_this, a1, a2);
	if (*(int*)a2 == 0x7)
	{
		Load();
		adv_cheese_map.emplace(a1, new AdvCheese());
	}
	return result;
}

void* ADV_PLAYER__dtor_r(void* _this, int flag)
{
	Destroy(_this, nullptr, flag);
	return ADV_PLAYER__dtor_h.Original(_this, flag);
}

void ADV_PL_Cheese()
{
	ADV_PLAYER__PDisp_h.Hook(ADV_PLAYER__PDisp_r);
	ADV_PLAYER__ctor_h.Hook(ADV_PLAYER__ctor_r);
	ADV_PLAYER__dtor_h.Hook(ADV_PLAYER__dtor_r);
}