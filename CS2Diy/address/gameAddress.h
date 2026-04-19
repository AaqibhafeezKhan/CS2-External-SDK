#pragma once
#include <Windows.h>
#include "../myimgui.h"

namespace gameAddress {
	inline HWND g_hwnd;
	inline char* clientAddress;
	inline DWORD g_pid;
	inline HANDLE g_handle;
	inline ImVec2 g_size;
	inline char* localAddress;
	inline int flag;
	inline int jumpOn = 65537;
	inline int jumpOff = 16777472;
}
