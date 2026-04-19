#pragma once

#include "myimgui.h"
#include <chrono>

inline bool showMenu = true;
void ShowImguiMenu();

namespace Menu
{
	inline bool Aimbot=true;
	inline float Fov=22.f;
	inline bool DrawFov=true;
	inline int Aimplace=6;
	inline int Aimkey= VK_SHIFT;

	inline bool Team;
	inline int box=1;
	inline bool Adaptivebox;
	inline bool Health;
	inline bool Skeleton = false;
	inline int SkeletonType = 3;
	inline bool FovEnable = false;
	inline int FovValue = 90;
	inline bool SpinEnable;
}
