#pragma once
#include <cmath>
#include <cstdint>

// Retail GC 800EC828. The PC helper 5937B0 instead projects the old heading
// onto the new floor and rewrites yaw. On a twisting path that removes the
// sideways velocity on which the ordinary movement friction acts.
inline void ConsoleFloorAlignment(const int32_t* previous, float* normal, float* degrees) {
    constexpr float unitsPerRadian = 10430.380859375f; // GC 8042E29C
    constexpr float unitsPerDegree = 65536.0f / 360.0f;
    int pitch, roll, yaw = static_cast<int>(degrees[1] * unitsPerDegree);
    const float x=normal[0], y=normal[1], z=normal[2];
    if (z > .9658f || (z > .95f && std::abs(y) < .0009f)) {
        pitch=0x4000; roll=previous[2]; normal[0]=normal[1]=0; normal[2]=1;
    } else if (z < -.9658f || (z < -.95f && std::abs(y) < .0009f)) {
        pitch=0xc000; roll=previous[2]; normal[0]=normal[1]=0; normal[2]=-1;
    } else {
        if (y > .99608f) {
            pitch=roll=0; normal[0]=normal[2]=0; normal[1]=1;
        } else if (y < -.99608f) {
            pitch=0x8000; roll=0; normal[0]=normal[2]=0; normal[1]=-1;
        } else {
            pitch=static_cast<int>(std::asin(z)*unitsPerRadian);
            roll=-static_cast<int>(std::atan2(x,y)*unitsPerRadian);
        }
        if (std::abs(static_cast<int>(static_cast<int16_t>(previous[2]-roll))) > 0x4000) {
            roll+=0x8000; pitch=0x8000-pitch;
        }
        if ((previous[0]==0x4000 || previous[0]==0xc000) && pitch!=0x4000 && pitch!=0xc000)
            yaw=previous[2]+yaw-roll;
    }
    degrees[0]=pitch*(360.0f/65536.0f);
    degrees[1]=yaw*(360.0f/65536.0f);
    degrees[2]=roll*(360.0f/65536.0f);
}

void RestorePathFloorAlignment();
