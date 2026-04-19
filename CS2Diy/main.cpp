#pragma once

#include "myimgui.h"
#include "cheat.h"
#include "tools/XorStr.h"
#include "address/gameAddress.h"
#include <ShlObj.h>

using namespace std;

bool isChinaRegion = false;

void RandomTitle()
{
	constexpr int length = 25;
	const auto characters = TEXT("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`-=~!@#$%^&*()_+,./;'[]|{}:?");
	TCHAR title[length + 1]{};
	for (int j = 0; j != length; j++)
	{
		title[j] += characters[rand() % 91];
	}
	SetConsoleTitle(title);
}

void initGame() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	srand((unsigned)time(NULL));
	RandomTitle();

	gameAddress::g_hwnd = FindWindowA(XorStr("SDL_app"), XorStr("Counter-Strike 2"));
	if (gameAddress::g_hwnd == NULL) {
		gameAddress::g_hwnd = FindWindowA(XorStr("SDL_app"), XorStr("反恐精英：全球攻势"));
		isChinaRegion = true;
	}

	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);
	std::cout << "HWND: " << gameAddress::g_hwnd << std::endl;

	GetWindowThreadProcessId(gameAddress::g_hwnd, &gameAddress::g_pid);

	gameAddress::g_handle = OpenProcess(PROCESS_ALL_ACCESS, true, gameAddress::g_pid);
	std::cout << "g_handle: " << gameAddress::g_handle << std::endl;

	if (gameAddress::clientAddress = mem::GetModule(gameAddress::g_pid, L"client.dll")) {
		std::cout << XorStr("Successfully found client.dll") << std::endl;
	}

	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	printf("PID: %d\n", gameAddress::g_pid);
	printf("client.dll Address: %p\n", gameAddress::clientAddress);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
}

int main()
{
	if (!IsUserAnAdmin()) {
		ShellExecute(NULL, L"runas", L"CS2-Cheat.exe", NULL, NULL, SW_SHOWNORMAL);
		cout << "Please run as administrator." << endl;
		return 1;
	}

	initGame();

	if (isChinaRegion)
	{
		std::cout << XorStr("Found China region window") << std::endl;
		myimgui::CreateWindow_Violet(XorStr("SDL_app"), "反恐精英：全球攻势", &fun, "C:\\Windows\\Fonts\\simkai.ttf",
			18.0f, false, 1);
	}
	else
	{
		myimgui::CreateWindow_Violet(XorStr("SDL_app"), "Counter-Strike 2", &fun, "C:\\Windows\\Fonts\\simkai.ttf",
			18.0f, false, 1);
	}
}
