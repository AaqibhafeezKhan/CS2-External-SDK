#pragma once
#include "tools/Mem.h"
#include "myimgui.h"
#include "Menu.h"
#include "tool.h"
#include "output/offsets.hpp"
#include "aimbot.h"
#include "esp.h"
#include "address/angle.h"

#include <cmath>
#include <iostream>
#include <vector>

struct D2D {
    float x, y;
};

struct D3D {
    float x, y, z;
};

float calculateDistance(const D2D crosshair, const D2D target);

namespace cheat {
    inline char* EnterAimAddress[2];
    inline float ActorDistance[2] = { 99999.f, 999999.f };

    inline float screenWidth;
    inline float screenHeight;

    inline HWND g_hwnd;
    inline char* clientAddress;
    inline DWORD g_pid;
    inline HANDLE g_handle;
    inline ImVec2 g_size;

    inline float Matrix[4][4];
    inline char* AimAddress[2];   
    inline float FOV;
    inline D2D Aimmouse;

    struct ActorInfo {
        char* Address[3];
        char* SkeletonAddress[2];
        int Health;
        int camp;
        D3D Axis;
        float x1;
        float y1; 
        float x2;
        float y2;
        float distance;
    };

    inline ActorInfo LocalPlayer;
    inline ActorInfo ActorPlayer;
    inline std::vector<ActorInfo> targets;
}

void fun();
void Traverse();

bool ReadLocalPawn();
bool ReadActorPawn();

bool WorldScreen2d(_In_ float world[3], _Out_ float screen[2]);

void DrawHealthBar(); 
void DrawSkeleton();
void DrawBoneLine(char* Address, int start, int End);
bool Update2DBox();
void DrawESPBox();
void DrawPlayerInfo();

D2D Aiming(D3D LocalAxis, char* AimAddress);
