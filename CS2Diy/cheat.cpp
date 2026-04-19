#include "cheat.h"

void fun() {
	if (!ReadLocalPawn()) return;
	Traverse();
	
	if (Menu::Aimbot) {
		aimbot();
	}
	
	if (Menu::DrawFov) {
		ImGui::GetForegroundDrawList()->AddCircle({ cheat::screenWidth / 2.0f, cheat::screenHeight / 2.0f }, Menu::Fov * 8, ImColor(255, 255, 255), 100);
	}

    if (Menu::FovEnable) {
        UpdateFOV();
    }
}

void Traverse() {
    mem::Read(gameAddress::g_handle, gameAddress::clientAddress + 0x1A33E30, &cheat::Matrix, 64);

    for (int i = 0; i < 64; i++) {
        if (!ReadActorPawn()) continue;
        
        if (cheat::ActorPlayer.Health <= 0) continue;
        
        if (!Menu::Team && cheat::ActorPlayer.camp == cheat::LocalPlayer.camp) continue;

        if (Update2DBox()) {
            if (Menu::box != 0) DrawESPBox();
            if (Menu::Health) DrawHealthBar();
            if (Menu::Skeleton) DrawSkeleton();
        }
        
        if (Menu::Aimbot) {
            UpdateAimbotQueue();
        }
    }
}

bool ReadLocalPawn() {
    if (!mem::Read(gameAddress::g_handle, gameAddress::clientAddress + 0x1836BB8, &cheat::LocalPlayer.Address[0], 8)) return false;
    mem::Read(gameAddress::g_handle, cheat::LocalPlayer.Address[0] + 0xB5C, &cheat::LocalPlayer.Health, 4);
    mem::Read(gameAddress::g_handle, cheat::LocalPlayer.Address[0] + 0x1588, &cheat::LocalPlayer.Axis, 12);
    return true;
}

bool ReadActorPawn() {
    // This needs proper implementation of entity list traversal
    // For now, it's a stub that should be updated with actual logic
    return false; 
}

bool WorldScreen2d(_In_ float world[3], _Out_ float screen[2]) {
    float w = cheat::Matrix[3][0] * world[0] + cheat::Matrix[3][1] * world[1] + cheat::Matrix[3][2] * world[2] + cheat::Matrix[3][3];

    if (w < 0.01f) return false;

    float x = cheat::Matrix[0][0] * world[0] + cheat::Matrix[0][1] * world[1] + cheat::Matrix[0][2] * world[2] + cheat::Matrix[0][3];
    float y = cheat::Matrix[1][0] * world[0] + cheat::Matrix[1][1] * world[1] + cheat::Matrix[1][2] * world[2] + cheat::Matrix[1][3];

    float invW = 1.0f / w;
    x *= invW;
    y *= invW;

    float halfWide = cheat::screenWidth * 0.5f;
    float halfHigh = cheat::screenHeight * 0.5f;

    screen[0] = halfWide * x + (halfWide);
    screen[1] = -(halfHigh * y) + (halfHigh);

    return true;
}

float calculateDistance(const D2D p1, const D2D p2) {
    return sqrtf(powf(p1.x - p2.x, 2) + powf(p1.y - p2.y, 2));
}
