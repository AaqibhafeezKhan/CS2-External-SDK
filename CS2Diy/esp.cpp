#include "esp.h"
#include "cheat.h"

void DrawESPBox() {
	ImDrawList* drawList = ImGui::GetForegroundDrawList();
	drawList->AddRect(
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y1 },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y2 },
		ImColor(255, 255, 255)
	);
}

void DrawHealthBar() {
    float healthPercentage = (float)cheat::ActorPlayer.Health / 100.0f;
    if (healthPercentage > 1.0f) healthPercentage = 1.0f;
    if (healthPercentage < 0.0f) healthPercentage = 0.0f;

    float boxHeight = cheat::ActorPlayer.y2 - cheat::ActorPlayer.y1;
    float barHeight = boxHeight * healthPercentage;

    ImGui::GetForegroundDrawList()->AddRectFilled(
        { cheat::ActorPlayer.x1 - 6, cheat::ActorPlayer.y2 - barHeight },
        { cheat::ActorPlayer.x1 - 2, cheat::ActorPlayer.y2 },
        ImColor(0, 255, 0)
    );
}

void DrawBoneLine(char* Address, int start, int End) {
    D3D startBone, endBone;
    float startScreen[2], endScreen[2];
    
    mem::Read(gameAddress::g_handle, Address + start * 32, &startBone, 12);
    mem::Read(gameAddress::g_handle, Address + End * 32, &endBone, 12);
    
    if (WorldScreen2d((float*)&startBone, startScreen) && WorldScreen2d((float*)&endBone, endScreen)) {
        ImGui::GetForegroundDrawList()->AddLine({ startScreen[0], startScreen[1] }, { endScreen[0], endScreen[1] }, ImColor(255, 255, 255));
    }
}

bool Update2DBox() {
	float foot[3] = { cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y ,cheat::ActorPlayer.Axis.z };
	float screen_foot[2];
	if (!WorldScreen2d(foot, screen_foot)) return false;

	float head[3] = { cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y ,cheat::ActorPlayer.Axis.z + 75.f };
	float screen_head[2];
	if (!WorldScreen2d(head, screen_head)) return false;

	float height = screen_foot[1] - screen_head[1];
	float width = height / 2.0f;

	cheat::ActorPlayer.x1 = screen_head[0] - width / 2.0f;
	cheat::ActorPlayer.y1 = screen_head[1];
	cheat::ActorPlayer.x2 = screen_head[0] + width / 2.0f;
	cheat::ActorPlayer.y2 = screen_foot[1];

	return true;
}

void DrawSkeleton() {
    char* address = cheat::ActorPlayer.SkeletonAddress[1];
    if (address == nullptr) return;

    DrawBoneLine(address, 7, 6);
    DrawBoneLine(address, 6, 1);
    DrawBoneLine(address, 6, 14);
    DrawBoneLine(address, 14, 15);
    DrawBoneLine(address, 15, 16);
    DrawBoneLine(address, 6, 62);
    DrawBoneLine(address, 62, 10);
    DrawBoneLine(address, 10, 11);
    DrawBoneLine(address, 1, 26);
    DrawBoneLine(address, 26, 27);
    DrawBoneLine(address, 27, 28);
    DrawBoneLine(address, 1, 23);
    DrawBoneLine(address, 23, 24);
    DrawBoneLine(address, 24, 25);
}
