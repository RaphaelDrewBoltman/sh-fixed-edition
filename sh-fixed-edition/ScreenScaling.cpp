#include "pch.h"
#include "ScreenScaling.h"
#include <cstdint>
#include <cstring>
#include <vector>
#include <algorithm>
#if defined(_M_IX86)
namespace {
struct Vertex { float x,y,z,rhw; uint32_t color; float u,v; };
static_assert(sizeof(Vertex)==28);
struct Transform { float sx=1,sy=1,ox=0,oy=0; };
static thread_local Transform current;
static int creditsLogicalHeight=480;
static const uintptr_t creditsOriginal=0x4546c0;
static float WidthScale(){return *reinterpret_cast<int*>(0xa7793c)/640.0f;}
static float HeightScale(){return *reinterpret_cast<int*>(0xa77940)/480.0f;}
using DrawFn=int(__cdecl*)(int,Vertex*,int);
static DrawFn chainedDraw=nullptr;
static float UniformScale(){return (std::min)(WidthScale(),HeightScale());}
static int DrawWith(int type,Vertex* vertices,int count,Transform t){
 auto driver=chainedDraw ? chainedDraw : *reinterpret_cast<DrawFn*>(*reinterpret_cast<uintptr_t*>(0x8e0a4c)+0x30);
 if(!vertices || count<=0 || (t.sx==1&&t.sy==1&&t.ox==0&&t.oy==0))return driver(type,vertices,count);
 // Copy because some callers retain their source vertices between draws.
 std::vector<Vertex> copy(vertices,vertices+count);
 for(auto& v:copy){v.x=v.x*t.sx+t.ox;v.y=v.y*t.sy+t.oy;}
 return driver(type,copy.data(),count);
}
static int __cdecl Draw(int type,Vertex* vertices,int count){return DrawWith(type,vertices,count,current);}
static int __cdecl LensDraw(int type,Vertex* vertices,int count){
 if(!vertices || count<=0)return DrawWith(type,vertices,count,{});
 // Each lens particle is one quad. Scale its center with the viewport,
 // but use a uniform scale for its radius so circles remain circles.
 float cx=0,cy=0;
 for(int i=0;i<count;++i){cx+=vertices[i].x;cy+=vertices[i].y;}
 cx/=count;cy/=count;const float scale=UniformScale();
 return DrawWith(type,vertices,count,{scale,scale,cx*(WidthScale()-scale),cy*(HeightScale()-scale)});
}
// Render resolution-scaled bounds in logical pixels before fitting the pass to the viewport.
static void __fastcall Prompt(void* object,void*){
 float sx=WidthScale(),sy=HeightScale();
 if(sx<=0||sy<=0)return;
 auto p=static_cast<unsigned char*>(object);const int offsets[]={0x3c,0x40,0x48,0x4c};float saved[4];
 for(int i=0;i<4;++i){memcpy(&saved[i],p+offsets[i],4);*reinterpret_cast<float*>(p+offsets[i])=saved[i]/(i%2?sy:sx);}
 const float scale=UniformScale();
 const Transform old=current;current={scale,scale,320.0f*(sx-scale),240.0f*(sy-scale)};
 reinterpret_cast<void(__fastcall*)(void*,void*)>(0x4570d0)(object,nullptr);
 current=old;for(int i=0;i<4;++i)memcpy(p+offsets[i],&saved[i],4);
}
// Preserve the horizontal anchor, which already follows the screen width.
static void __cdecl CreditsPass(void* object){
 const Transform old=current;float sx=UniformScale();float anchor=*reinterpret_cast<float*>(static_cast<char*>(object)+0x8c);
 current={sx,sx,anchor*(1-sx),240.0f*(HeightScale()-sx)};
 __asm { mov ebx,object
         call dword ptr [creditsOriginal] }
 current=old;
}
__declspec(naked) static void Credits(){__asm {push ebx
 call CreditsPass
 add esp,4
 ret}}
static bool Write(uintptr_t address,const void* data,size_t n){DWORD old;if(!VirtualProtect(reinterpret_cast<void*>(address),n,PAGE_EXECUTE_READWRITE,&old))return false;memcpy(reinterpret_cast<void*>(address),data,n);FlushInstructionCache(GetCurrentProcess(),reinterpret_cast<void*>(address),n);DWORD ignored;return VirtualProtect(reinterpret_cast<void*>(address),n,old,&ignored)!=0;}
static bool Branch(uintptr_t address,void* target,unsigned char opcode){unsigned char code[5]={opcode};int32_t rel=int32_t(reinterpret_cast<uintptr_t>(target)-address-5);memcpy(code+1,&rel,4);return Write(address,code,5);}
} // namespace
#endif

void RestoreScreenScaling(){
#if defined(_M_IX86)
 static bool installed=false;if(installed)return;
 const unsigned char draw[]={0xa1,0x4c,0x0a,0x8e,0};const unsigned char credits[]={0xe8,0xa9,0,0,0};const unsigned char lens[]={0xe8,0x86,0xd4,0x1b,0};const unsigned char height[]={0xdb,0x05,0x40,0x79,0xa7,0};
 if(GetModuleHandleW(nullptr)!=reinterpret_cast<HMODULE>(0x400000) || (memcmp(reinterpret_cast<void*>(0x64ca10),draw,5) && *reinterpret_cast<unsigned char*>(0x64ca10)!=0xe9)||memcmp(reinterpret_cast<void*>(0x454612),credits,5)||memcmp(reinterpret_cast<void*>(0x48f585),lens,5)||memcmp(reinterpret_cast<void*>(0x4546f5),height,6)||*reinterpret_cast<uintptr_t*>(0x75024c)!=0x4570d0){return;}
 // Preserve an existing widescreen hook rather than bypassing its HUD logic.
 if(*reinterpret_cast<unsigned char*>(0x64ca10)==0xe9)
  chainedDraw=reinterpret_cast<DrawFn>(0x64ca15+*reinterpret_cast<int32_t*>(0x64ca11));
 uintptr_t prompt=reinterpret_cast<uintptr_t>(&Prompt),h=reinterpret_cast<uintptr_t>(&creditsLogicalHeight);
 installed=Branch(0x64ca10,reinterpret_cast<void*>(&Draw),0xe9)&&Write(0x75024c,&prompt,4)&&Branch(0x454612,reinterpret_cast<void*>(&Credits),0xe8)&&Write(0x4546f7,&h,4)&&Branch(0x48f585,reinterpret_cast<void*>(&LensDraw),0xe8);
#endif
}
