#pragma once
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

// Read the original PRS-compressed model/animation from the installed ONE archive.
// No extracted game assets need to be distributed with the mod.
inline std::vector<unsigned char> ReadCheeseAsset(const std::string& path, const char* name) {
 std::ifstream input(path,std::ios::binary);
 if (!input) return {};
 std::vector<unsigned char> archive((std::istreambuf_iterator<char>(input)),{});
 auto word=[&](size_t at) {
  if (at>archive.size() || archive.size()-at<4) throw std::runtime_error("Truncated ONE archive");
  uint32_t value; std::memcpy(&value,archive.data()+at,4); return value;
 };
 for (size_t chunk=0x4018;chunk+12<=archive.size();) {
  const uint32_t index=word(chunk), length=word(chunk+4);
  if (length>archive.size()-chunk-12 || index>=256) throw std::runtime_error("Invalid ONE chunk");
  const size_t entry=24+size_t(index)*64;
  if (entry+64>archive.size()) throw std::runtime_error("Truncated ONE name");
  if (std::strlen(name)<64 && std::memcmp(archive.data()+entry,name,std::strlen(name)+1)==0) {
   size_t cursor=chunk+12, end=cursor+length;
   unsigned bits=0, remaining=0;
   auto byte=[&]() -> unsigned {
    if(cursor==end) throw std::runtime_error("Truncated PRS data");
    return archive[cursor++];
   };
   auto bit=[&]() {
    if (!remaining) { bits=byte(); remaining=8; }
    unsigned value=bits&1; bits>>=1; --remaining; return value;
   };
   std::vector<unsigned char> output;
   for (;;) {
    // These small assets never approach this bound; reject malformed streams.
    if (output.size()>16*1024*1024) throw std::runtime_error("Oversized PRS data");
    if (bit()) { output.push_back(static_cast<unsigned char>(byte())); continue; }
    int offset; unsigned count;
    if (bit()) {
     unsigned code=byte(); code|=byte()<<8;
     if (!code) return output;
     offset=int(code>>3)-8192; count=code&7;
     count=count ? count+2 : byte()+1;
    } else {
     count=bit()<<1; count|=bit(); count+=2; offset=int(byte())-256;
    }
    if (size_t(-offset)>output.size()) throw std::runtime_error("Invalid PRS back reference");
    while(count--) output.push_back(output[output.size()+offset]);
   }
  }
  chunk+=12+length;
 }
 return {};
}
