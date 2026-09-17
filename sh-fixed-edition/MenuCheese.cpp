#include "pch.h"
#include "MenuCheese.h"
#include "MenuCheeseAssets.h"
#undef min
#undef max
#include <windows.h>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <string>
#include <unordered_map>


template<class T> static T& At(void* p, size_t offset) { return *reinterpret_cast<T*>(static_cast<char*>(p)+offset); }
template<class R, class... A> static R Call(uintptr_t address, A... args) { return reinterpret_cast<R(__cdecl*)(A...)>(address)(args...); }
namespace {
static double AdvanceMenuAnimation(double seconds,uint32_t updates,double duration,bool loop) {
 const double next=seconds+updates/60.0;
 return loop ? std::fmod(next,duration) : std::min(next,duration);
}
static void* textures;
static void* animations[4];
static bool attempted;
struct Actor { void* clump=nullptr; void* hierarchy=nullptr; void* ball=nullptr; void* ballAnchor=nullptr; int pose=-1; uint32_t tick=0; double seconds=0; };
static std::unordered_map<void*,Actor> actors;

static void* ReadAsset(const char* name, unsigned type, uintptr_t reader) {
 char path[MAX_PATH];
 if (!GetModuleFileNameA(nullptr,path,MAX_PATH)) return nullptr;
 std::string file(path); file.resize(file.find_last_of("\\/")+1);
 file += "dvdroot/playmodel/cheese.one";
 std::vector<unsigned char> bytes;
 try { bytes=ReadCheeseAsset(file,name); } catch (const std::exception&) { return nullptr; }
 if (bytes.empty()) return nullptr;
 struct Memory { void* start; unsigned length; } memory={bytes.data(),static_cast<unsigned>(bytes.size())};
 void* stream=Call<void*>(0x64dce0,3,1,&memory);
 if (!stream) return nullptr;
 void* result=nullptr;
 if (Call<int>(0x64d4c0,stream,type,nullptr,nullptr)) result=Call<void*>(reader,stream);
 Call<int>(0x64dc10,stream,nullptr);
 return result;
}
static bool Load() {
 if (attempted) return textures && animations[0] && animations[1] && animations[2] && animations[3];
 attempted=true;
 textures=ReadAsset("CHEESE.TXD",0x16,0x653340);
 const char* names[]={"CH_CHARANGE_IDLE.ANM","CH_CHARANGE_SELECT.ANM","CH_STORY_IDLE.ANM","CH_STORY_SELECT.ANM"};
 for (int i=0;i<4;++i) animations[i]=ReadAsset(names[i],0x1b,0x69dd50);
 return Load();
}
static void* __cdecl FindHierarchy(void* frame, void* data) {
 auto found=static_cast<void**>(data);
 if (!*found) *found=Call<void*>(0x6a8640,frame);
 if (!*found) Call<void*>(0x64c5e0,frame,&FindHierarchy,data);
 return *found ? nullptr : frame;
}
static void* __cdecl BindSkin(void* atomic, void* hierarchy) {
 Call<void*>(0x6a06b0,atomic,hierarchy);
 return atomic;
}
static Actor Create() {
 Actor a;
 void* previous=Call<void*>(0x64e450);
 Call<void*>(0x64e430,textures);
 a.clump=ReadAsset("CHAO_LOCATOR.DFF",0x10,0x66c250);
 if (a.clump) a.ball=ReadAsset("CH_KAKU_LOCATOR.DFF",0x10,0x66c250);
 Call<void*>(0x64e430,previous);
 if (!a.clump) return a;
 FindHierarchy(At<void*>(a.clump,4),&a.hierarchy);
 if (!a.hierarchy) { if(a.ball) Call<int>(0x66bc60,a.ball); Call<int>(0x66bc60,a.clump); return {}; }
 At<unsigned>(a.hierarchy,0) |= 0x3000;
 Call<void*>(0x66b540,a.clump,&BindSkin,a.hierarchy);
 Call<int>(0x69dfc0,At<void*>(a.hierarchy,0x20),animations[0]);
 Call<int>(0x6a8560,a.hierarchy);
 // Gameplay attaches the separate ball to hierarchy node ID 1.
 auto nodes=static_cast<unsigned char*>(At<void*>(a.hierarchy,0x10));
 for (int i=0;i<At<int>(a.hierarchy,4);++i)
  if (At<int>(nodes+i*16,0)==1) a.ballAnchor=At<void*>(nodes+i*16,12);
 if (a.ball && !a.ballAnchor) { Call<int>(0x66bc60,a.ball); a.ball=nullptr; }
 if (a.ball) {
  Call<void*>(0x66b540,a.ball,reinterpret_cast<void*>(0x581f70),nullptr);
  Call<void*>(0x669840,*reinterpret_cast<void**>(0x8d69cc),a.ball);
 }
 // Use the same material/pipeline setup and world lights as the menu characters.
 Call<void*>(0x66b540,a.clump,reinterpret_cast<void*>(0x581f70),nullptr);
 Call<void*>(0x669840,*reinterpret_cast<void**>(0x8d69cc),a.clump);
 return a;
}
static void __cdecl RenderCheese(void* player) {
 if (At<int>(player,0x190)!=7 || At<int>(player,0x194)<3 || !Load()) return;
 auto entry=actors.find(player);
 if (entry==actors.end()) entry=actors.emplace(player,Create()).first;
 Actor& a=entry->second;
 if (!a.clump) return;
 const int index=At<int>(player,0x40);
 if (index<0 || index>=48 || index%12!=7) return;
 const int pose=index/12;
 void* interpolator=At<void*>(a.hierarchy,0x20);
 const float duration=At<float>(animations[pose],0xc);
 if (!std::isfinite(duration) || duration<=0) return;
 const uint32_t tick=At<uint32_t>(player,0x194);
 if (a.pose!=pose) {
  Call<int>(0x69dfc0,interpolator,animations[pose]);
  a.pose=pose; a.tick=tick; a.seconds=0;
 }
 // Count gameplay updates, not renders or Cream's shorter animation cycles.
 a.seconds=AdvanceMenuAnimation(a.seconds,tick-a.tick,duration,pose%2==0);
 a.tick=tick;
 Call<int>(0x69e490,interpolator,static_cast<float>(a.seconds));
 void* root=At<void*>(a.clump,4);
 void* creamRoot=At<void*>(At<void*>(player,0xc0),4);
 memcpy(static_cast<char*>(root)+0x10,static_cast<char*>(creamRoot)+0x10,64);
 // Challenge animations include a translation; story animations are centered
 // at zero and need a separate hovering position beside Cream.
 // In the story camera, negative local X moves right on screen.
 if (pose>=2) {
  float* matrix=reinterpret_cast<float*>(static_cast<char*>(root)+0x10);
  for (int axis=0;axis<3;++axis)
   matrix[12+axis] += -3.5f*matrix[axis] - 4.0f*matrix[4+axis];
  // Move the whole model toward the active camera along its projection rays.
  // Scaling translation and basis together preserves screen position and size.
  void* camera=At<void*>(*reinterpret_cast<void**>(0x8e0a4c),0);
  if (camera) {
   void* cameraFrame=At<void*>(camera,4);
   const float* cameraMatrix=Call<const float*>(0x64c300,cameraFrame);
   constexpr float depthScale=0.35f;
   for (int axis=0;axis<3;++axis) {
    matrix[12+axis]=cameraMatrix[12+axis]+depthScale*(matrix[12+axis]-cameraMatrix[12+axis]);
    matrix[axis]*=depthScale;
    matrix[4+axis]*=depthScale;
    matrix[8+axis]*=depthScale;
   }
   // The scaled basis is no longer orthonormal or identity.
   reinterpret_cast<unsigned*>(matrix)[3] &= ~0x20003u;
  }
 }
 Call<void*>(0x64c280,root);
 Call<int>(0x6a8660,a.hierarchy);
 Call<void*>(0x66b4f0,a.clump);
 if (a.ball) {
  void* ballRoot=At<void*>(a.ball,4);
  const float* anchor=Call<const float*>(0x64c300,a.ballAnchor);
  float* matrix=reinterpret_cast<float*>(static_cast<char*>(ballRoot)+0x10);
  memcpy(matrix,anchor,64);
  // Original gameplay offset (0, 3.41, 0.17), in the animated anchor's space.
  // Its basis also carries the story-menu perspective compensation.
  for (int axis=0;axis<3;++axis)
   matrix[12+axis]+=3.41f*matrix[4+axis]+0.17f*matrix[8+axis];
  reinterpret_cast<unsigned*>(matrix)[3] &= ~0x20000u;
  Call<void*>(0x64c280,ballRoot);
  Call<void*>(0x66b4f0,a.ball);
 }
}
static const uintptr_t originalDraw=0x66b4f0;
__declspec(naked) static void Draw() { __asm {
 push dword ptr [esp+4]
 call dword ptr [originalDraw]
 add esp,4
 push eax
 push esi
 call RenderCheese
 add esp,4
 pop eax
 ret
} }
static void* __fastcall Destroy(void* player,void*,unsigned flags) {
 auto entry=actors.find(player);
 if (entry!=actors.end()) {
  if (entry->second.ball) {
   Call<void*>(0x6699a0,*reinterpret_cast<void**>(0x8d69cc),entry->second.ball);
   Call<int>(0x66bc60,entry->second.ball);
  }
  if (entry->second.clump) {
   Call<void*>(0x6699a0,*reinterpret_cast<void**>(0x8d69cc),entry->second.clump);
   Call<int>(0x66bc60,entry->second.clump);
  }
  actors.erase(entry);
 }
 return reinterpret_cast<void*(__thiscall*)(void*,unsigned)>(0x453330)(player,flags);
}
static bool Write(uintptr_t address,const void* data,size_t size) {
 DWORD old;
 if (!VirtualProtect(reinterpret_cast<void*>(address),size,PAGE_EXECUTE_READWRITE,&old)) return false;
 memcpy(reinterpret_cast<void*>(address),data,size);
 FlushInstructionCache(GetCurrentProcess(),reinterpret_cast<void*>(address),size);
 DWORD ignored; VirtualProtect(reinterpret_cast<void*>(address),size,old,&ignored);
 return true;
}
}

void RestoreMenuCheese() {
 static bool installed=false;
 if (installed) return;
 const unsigned char expected[]={0xe8,0x71,0x7f,0x21,0};
 if (GetModuleHandleW(nullptr)!=reinterpret_cast<HMODULE>(0x400000) || memcmp(reinterpret_cast<void*>(0x45357a),expected,5) || *reinterpret_cast<uintptr_t*>(0x74f6cc)!=0x453330) {
  MessageBoxW(nullptr,L"Menu Cheese could not load: unexpected game code or a conflicting mod.",L"Menu Cheese",MB_OK|MB_ICONERROR); return;
 }
 unsigned char branch[5]={0xe8};
 int32_t relative=static_cast<int32_t>(reinterpret_cast<uintptr_t>(&Draw)-0x45357f);
 memcpy(branch+1,&relative,4);
 uintptr_t destructor=reinterpret_cast<uintptr_t>(&Destroy);
 installed=Write(0x74f6cc,&destructor,4) && Write(0x45357a,branch,5);
}
