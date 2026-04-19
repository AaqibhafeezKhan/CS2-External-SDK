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
    DrawBoneLine(cheat::ActorPlayer.SkeletonAddress[1], 7, 6);
    DrawBoneLine(cheat::ActorPlayer.SkeletonAddress[1], 6, 1);
    DrawBoneLine(cheat::ActorPlayer.SkeletonAddress[1], 6, 14);
    DrawBoneLine(cheat::ActorPlayer.SkeletonAddress[1], 14, 15);
    DrawBoneLine(cheat::ActorPlayer.SkeletonAddress[1], 15, 16);
    DrawBoneLine(cheat::ActorPlayer.SkeletonAddress[1], 6, 62);
    DrawBoneLine(cheat::ActorPlayer.SkeletonAddress[1], 62, 10);
    DrawBoneLine(cheat::ActorPlayer.SkeletonAddress[1], 10, 11);
    DrawBoneLine(cheat::ActorPlayer.SkeletonAddress[1], 1, 26);
    DrawBoneLine(cheat::ActorPlayer.SkeletonAddress[1], 26, 27);
    DrawBoneLine(cheat::ActorPlayer.SkeletonAddress[1], 27, 28);
    DrawBoneLine(cheat::ActorPlayer.SkeletonAddress[1], 1, 23);
    DrawBoneLine(cheat::ActorPlayer.SkeletonAddress[1], 23, 24);
    DrawBoneLine(cheat::ActorPlayer.SkeletonAddress[1], 24, 25);
}
