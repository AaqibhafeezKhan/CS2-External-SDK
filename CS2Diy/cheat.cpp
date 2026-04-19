#include "cheat.h"

void fun() {
	if (!ReadLocalPawn()) return;
	Traverse();
	
	if (Menu::Aimbot) {
		aimbot();
	}
	
	if (Menu::DrawFov) {
		ImGui::GetForegroundDrawList()->AddCircle({ cheat::screenWidth, cheat::screenHeight }, Menu::Fov * 8, ImColor(255, 255, 255), 100);
	}

    if (Menu::FovEnable) {
        mem::Write(gameAddress::clientAddress + 0x1836BB8 + 0x1234, &Menu::FovValue, 4);
    }
}

void Traverse() {
    mem::Read(gameAddress::clientAddress + 0x1A33E30, &cheat::Matrix, 64);

    for (int i = 0; i < 64; i++) {
        if (!ReadActorPawn()) continue;
        
        if (cheat::ActorPlayer.Health <= 0) continue;
        
        if (!Menu::Team && cheat::ActorPlayer.camp == cheat::LocalPlayer.camp) continue;

        if (Update2DBox()) {
            if (Menu::box == 1) DrawESPBox();
            if (Menu::Health) DrawHealthBar();
            if (Menu::Skeleton) DrawSkeleton();
        }
        
        if (Menu::Aimbot) {
            UpdateAimbotQueue();
        }
    }
}

bool ReadLocalPawn() {
    if (!mem::Read(gameAddress::clientAddress + 0x1836BB8, &cheat::LocalPlayer.Address[0], 8)) return false;
    mem::Read(cheat::LocalPlayer.Address[0] + 0xB5C, &cheat::LocalPlayer.Health, 4);
    mem::Read(cheat::LocalPlayer.Address[0] + 0x1588, &cheat::LocalPlayer.Axis, 12);
    return true;
}

bool ReadActorPawn() {
    return true;
}

bool WorldScreen2d(_In_ float world[3], _Out_ float screen[2]) {
    float x = cheat::Matrix[0][0] * world[0] + cheat::Matrix[0][1] * world[1] + cheat::Matrix[0][2] * world[2] + cheat::Matrix[0][3];
    float y = cheat::Matrix[1][0] * world[0] + cheat::Matrix[1][1] * world[1] + cheat::Matrix[1][2] * world[2] + cheat::Matrix[1][3];
    float w = cheat::Matrix[3][0] * world[0] + cheat::Matrix[3][1] * world[1] + cheat::Matrix[3][2] * world[2] + cheat::Matrix[3][3];

    if (w < 0.01f) return false;

    float invW = 1.0f / w;
    x *= invW;
    y *= invW;

    float halfWide = cheat::screenWidth * 0.5f;
    float halfHigh = cheat::screenHeight * 0.5f;

    screen[0] = halfWide * x + (halfWide);
    screen[1] = -(halfHigh * y) + (halfHigh);

    return true;
}

bool Update2DBox() {
    float head[3] = { cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y, cheat::ActorPlayer.Axis.z + 75.f };
    float screen_head[2], screen_foot[2];
    
    float foot_array[3] = { cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y, cheat::ActorPlayer.Axis.z };
    if (!WorldScreen2d(foot_array, screen_foot)) return false;
    if (!WorldScreen2d(head, screen_head)) return false;

    float height = screen_foot[1] - screen_head[1];
    float width = height / 2.0f;

    cheat::ActorPlayer.x1 = screen_head[0] - width / 2.0f;
    cheat::ActorPlayer.y1 = screen_head[1];
    cheat::ActorPlayer.x2 = screen_head[0] + width / 2.0f;
    cheat::ActorPlayer.y2 = screen_foot[1];

    return true;
}

void DrawHealthBar() {
    float healthPercentage = cheat::ActorPlayer.Health / 100.0f;
    float boxHeight = cheat::ActorPlayer.y2 - cheat::ActorPlayer.y1;
    float barHeight = boxHeight * healthPercentage;

    ImGui::GetForegroundDrawList()->AddRectFilled(
        { cheat::ActorPlayer.x1 - 6, cheat::ActorPlayer.y2 - barHeight },
        { cheat::ActorPlayer.x1 - 2, cheat::ActorPlayer.y2 },
        ImColor(0, 255, 0)
    );
}

void DrawESPBox() {
    ImGui::GetForegroundDrawList()->AddRect(
        { cheat::ActorPlayer.x1, cheat::ActorPlayer.y1 },
        { cheat::ActorPlayer.x2, cheat::ActorPlayer.y2 },
        ImColor(255, 255, 255)
    );
}

float calculateDistance(const D2D p1, const D2D p2) {
    return sqrtf(powf(p1.x - p2.x, 2) + powf(p1.y - p2.y, 2));
}
