#include "Menu.h"

static int Buttonthis = 1;

void ShowImguiMenu()
{
    static std::chrono::time_point LastTimePoint = std::chrono::steady_clock::now();
    auto CurTimePoint = std::chrono::steady_clock::now();
    if (GetAsyncKeyState(VK_INSERT) &&
        (CurTimePoint - LastTimePoint >= std::chrono::milliseconds(200)))
    {
        showMenu = !showMenu;
        LastTimePoint = CurTimePoint;
    }

    if (showMenu)
    {
        ImGui::Begin(u8"CS2 External Cheat - Basic Edition");

        if (ImGui::Button(u8"Aimbot", { 80,20 }))
        {
            Buttonthis = 1;
        }
        ImGui::SameLine(NULL, 15.f);
        if (ImGui::Button(u8"Visuals", { 80,20 }))
        {
            Buttonthis = 2;
        }
        ImGui::SameLine(NULL, 15.f);
        if (ImGui::Button(u8"Misc", { 80,20 }))
        {
            Buttonthis = 3;
        }

        if (Buttonthis == 1)
        {
            ImGui::TextColored(ImColor(255, 0, 0, 255), u8"Aimbot Settings (+)");
            ImGui::Checkbox(u8"Enable Aimbot", &Menu::Aimbot);
            ImGui::SliderFloat(u8"FOV", (float*)&Menu::Fov, 1.f, 180.f, "%.0f");
            ImGui::Checkbox(u8"Draw FOV Circle", &Menu::DrawFov);
            ImGui::Separator();
            ImGui::Text(u8"Target Bone:");
            ImGui::RadioButton(u8"Head", &Menu::Aimplace, 6);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"Chest", &Menu::Aimplace, 3);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"Stomach", &Menu::Aimplace, 1);
            
            ImGui::Separator();
            ImGui::Text(u8"Hotkey:");
            ImGui::RadioButton(u8"Side Button (M5)", &Menu::Aimkey, 5);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"Left Click", &Menu::Aimkey, 1);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"Shift", &Menu::Aimkey, VK_SHIFT);
        }
        if (Buttonthis == 2)
        {
            ImGui::TextColored(ImColor(0, 217, 255), u8"Visual Settings (O)");
            ImGui::Checkbox(u8"Show Team", &Menu::Team);
            ImGui::Separator();
            ImGui::Text(u8"Box Type:");
            ImGui::RadioButton(u8"Off", &Menu::box, 0);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"Corner Box", &Menu::box, 1);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"3D Box", &Menu::box, 2);
            
            ImGui::Separator();
            ImGui::Checkbox(u8"Health Bar", &Menu::Health);
            ImGui::Checkbox(u8"Skeleton ESP", &Menu::Skeleton);
            
            ImGui::Separator();
            ImGui::Checkbox(u8"Override In-Game FOV", &Menu::FovEnable);
            if (Menu::FovEnable) {
                ImGui::SliderInt(u8"FOV Value", &Menu::FovValue, 10, 170);
            }
        }
        if (Buttonthis == 3)
        {
            ImGui::TextColored(ImColor(128, 128, 128), u8"Miscellaneous Settings (?)");
            ImGui::Text(u8"Skeleton Style:");
            ImGui::RadioButton(u8"Developer", &Menu::SkeletonType, 0);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"Default", &Menu::SkeletonType, 3);
            
            ImGui::Separator();
            ImGui::Checkbox(u8"Experimental Spinbot", &Menu::SpinEnable);
        }
        ImGui::End();
    }
}
