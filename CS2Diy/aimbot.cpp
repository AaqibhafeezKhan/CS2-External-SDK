#include "cheat.h"

void UpdateAimbotQueue()
{
	float World[3] = { cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y, cheat::ActorPlayer.Axis.z + 69.f };
	float screen[2];
	
	if (WorldScreen2d(World, screen))
	{
		cheat::ActorDistance[0] = calculateDistance({ cheat::screenWidth, cheat::screenHeight }, { screen[0], screen[1] });
		
		if (cheat::ActorDistance[0] <= cheat::ActorDistance[1])
		{
			cheat::ActorDistance[1] = cheat::ActorDistance[0];
			cheat::EnterAimAddress[0] = cheat::ActorPlayer.Address[1];
			cheat::EnterAimAddress[1] = cheat::ActorPlayer.SkeletonAddress[1];
		}
	}
}

void aimbot()
{
	if (cheat::LocalPlayer.Health <= 0) {
		return;
	}

	cheat::AimAddress[0] = cheat::EnterAimAddress[0];
	cheat::AimAddress[1] = cheat::EnterAimAddress[1];
	
	int target_hp;
	mem::Read(gameAddress::g_handle, cheat::AimAddress[0] + 0xB5C, &target_hp, 4);
	
	if (target_hp > 0)
	{
		cheat::FOV = Menu::Fov * 8;
		
		if (cheat::FOV > cheat::ActorDistance[1])
		{
			cheat::Aimmouse = Aiming(cheat::LocalPlayer.Axis, cheat::AimAddress[1]);

			if (GetAsyncKeyState(Menu::Aimkey))
			{
				mem::Write(gameAddress::clientAddress + ViewAngles::yaw, &cheat::Aimmouse.x, sizeof(cheat::Aimmouse.x));
				mem::Write(gameAddress::clientAddress + ViewAngles::pitch, &cheat::Aimmouse.y, sizeof(cheat::Aimmouse.y));
			}
		}
	}
	cheat::ActorDistance[1] = FLT_MAX;
}

D2D Aiming(D3D LocalAxis, char* AimAddress)
{
	char* Aimindex;
	D3D ActorAxis;
	D3D AimAxis;
	D2D Aimmouse;
	float P_I = 3.1415926535f;

	Aimindex = AimAddress + Menu::Aimplace * 32;
	
	mem::Read(gameAddress::g_handle, Aimindex, &ActorAxis.x, sizeof(ActorAxis.x));
	mem::Read(gameAddress::g_handle, Aimindex + 4, &ActorAxis.y, sizeof(ActorAxis.y));
	mem::Read(gameAddress::g_handle, Aimindex + 8, &ActorAxis.z, sizeof(ActorAxis.z));
	
	AimAxis.z = LocalAxis.z - ActorAxis.z;
	AimAxis.x = ActorAxis.x - LocalAxis.x;
	AimAxis.y = ActorAxis.y - LocalAxis.y;
	
	if (AimAxis.x != 0)
		Aimmouse.x = atan2(AimAxis.y, AimAxis.x) * 180 / P_I;

	if (AimAxis.z != 0)
		Aimmouse.y = atan(AimAxis.z / sqrt(AimAxis.x * AimAxis.x + AimAxis.y * AimAxis.y)) * 180 / P_I;

	return Aimmouse;
}

cheat::ActorInfo SelectNearestTarget(const std::vector<cheat::ActorInfo>& targets) {
	cheat::ActorInfo bestTarget{};
	float minDistance = FLT_MAX;

	for (const auto& target : targets) {
		if (target.distance < minDistance) {
			minDistance = target.distance;
			bestTarget = target;
		}
	}
	return bestTarget;
}
