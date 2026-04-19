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
    mem::Read(gameAddress::g_handle, gameAddress::clientAddress + cs2_dumper::offsets::client_dll::dwViewMatrix, &cheat::Matrix, 64);

    uintptr_t entity_list = 0;
    mem::Read(gameAddress::g_handle, gameAddress::clientAddress + cs2_dumper::offsets::client_dll::dwEntityList, &entity_list, 8);
    if (!entity_list) return;

    for (int i = 1; i < 64; i++) {
        uintptr_t list_entry = 0;
        mem::Read(gameAddress::g_handle, entity_list + (8 * (i >> 9) + 0x10), &list_entry, 8);
        if (!list_entry) continue;

        uintptr_t controller = 0;
        mem::Read(gameAddress::g_handle, list_entry + 120 * (i & 0x1FF), &controller, 8);
        if (!controller) continue;

        uint32_t pawn_handle = 0;
        mem::Read(gameAddress::g_handle, controller + 0x60C, &pawn_handle, 4);
        if (!pawn_handle) continue;

        uintptr_t list_entry_pawn = 0;
        mem::Read(gameAddress::g_handle, entity_list + (8 * ((pawn_handle & 0x7FFF) >> 9) + 0x10), &list_entry_pawn, 8);
        if (!list_entry_pawn) continue;

        uintptr_t pawn = 0;
        mem::Read(gameAddress::g_handle, list_entry_pawn + 120 * (pawn_handle & 0x1FF), &pawn, 8);
        if (!pawn) continue;

        cheat::ActorPlayer.Address[0] = (char*)controller;
        cheat::ActorPlayer.Address[1] = (char*)pawn;

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
    if (!mem::Read(gameAddress::g_handle, gameAddress::clientAddress + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn, &cheat::LocalPlayer.Address[1], 8)) return false;
    if (!cheat::LocalPlayer.Address[1]) return false;

    mem::Read(gameAddress::g_handle, cheat::LocalPlayer.Address[1] + 0x334, &cheat::LocalPlayer.Health, 4);
    mem::Read(gameAddress::g_handle, cheat::LocalPlayer.Address[1] + 0x3CB, &cheat::LocalPlayer.camp, 1);
    mem::Read(gameAddress::g_handle, cheat::LocalPlayer.Address[1] + 0x1274, &cheat::LocalPlayer.Axis, 12);
    
    return true;
}

bool ReadActorPawn() {
    if (!cheat::ActorPlayer.Address[1]) return false;

    mem::Read(gameAddress::g_handle, cheat::ActorPlayer.Address[1] + 0x334, &cheat::ActorPlayer.Health, 4);
    mem::Read(gameAddress::g_handle, cheat::ActorPlayer.Address[1] + 0x3CB, &cheat::ActorPlayer.camp, 1);
    mem::Read(gameAddress::g_handle, cheat::ActorPlayer.Address[1] + 0x1274, &cheat::ActorPlayer.Axis, 12);

    uintptr_t game_scene = 0;
    mem::Read(gameAddress::g_handle, cheat::ActorPlayer.Address[1] + 0x318, &game_scene, 8);
    if (game_scene) {
        mem::Read(gameAddress::g_handle, game_scene + 0x1F0, &cheat::ActorPlayer.SkeletonAddress[1], 8);
    }

    return true;
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
