#include "tool.h"

void UpdateFOV() {
    Utils::Process cs2(TEXT("cs2.exe"));
    
    auto client = cs2.get_module_handle(TEXT("client.dll"));
    std::cout << XorStr("Module found: ") << client << std::endl;
    
    auto local_player = cs2.read<Utils::addr64>(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
    
    auto cam_ser = cs2.read<Utils::addr64>(local_player + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pCameraServices);
    
    auto fov_addr = cam_ser + cs2_dumper::schemas::client_dll::CCSPlayerBase_CameraServices::m_iFOV;

    mem::Write((CHAR*)fov_addr, &Menu::FovValue, sizeof(Menu::FovValue));
}

void SpinBot() {
    float turnSpeed = 1.f;
    mem::Write(gameAddress::clientAddress + ViewAngles::yaw, &Utils::InitialValue, sizeof(Utils::InitialValue));
    Utils::InitialValue = Utils::InitialValue + turnSpeed;
    if (Utils::InitialValue >= 180.0f)
    {
        Utils::InitialValue = -180.0f;
    }
}

void BunnyHop() {
    Sleep(10);
    if (gameAddress::flag == 65665)
    {
        mem::Write(gameAddress::clientAddress + buttons::jump, &gameAddress::jumpOn, sizeof(gameAddress::jumpOn));
    }
    if (gameAddress::flag == 65664)
    {
        mem::Write(gameAddress::clientAddress + buttons::jump, &gameAddress::jumpOff, sizeof(gameAddress::jumpOff));
    }
}

void CheatThread() {
    while (true)
    {
        if (Menu::SpinEnable)
        {
            SpinBot();
        }
        mem::Read(gameAddress::g_handle, gameAddress::clientAddress + offsets::client_dll::dwLocalPlayerPawn, &gameAddress::localAddress, sizeof(gameAddress::localAddress));
        mem::Read(gameAddress::g_handle, gameAddress::localAddress + schemas::client_dll::C_BaseEntity::m_fFlags, &gameAddress::flag, sizeof(gameAddress::flag));
        
        if (GetAsyncKeyState(VK_SPACE)) {
            BunnyHop();
        }
        else {
            mem::Write(gameAddress::clientAddress + buttons::jump, &gameAddress::jumpOff, sizeof(gameAddress::jumpOff));
        }
        
    }
}
