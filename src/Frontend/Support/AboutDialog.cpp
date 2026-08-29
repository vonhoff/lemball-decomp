#include "AboutDialog.h"

#include "../../Platform/Windows/Entry.h"
#include "../../Visos/Foundation/VsMem.h"
#include "../../Visos/Sound/SoundManager.h"

// GLOBAL: LEMBALL 0x004a05ec
int g_nVisosBuildNumber = 201;

// GLOBAL: LEMBALL 0x004a05f0
char g_szWindowsNtBuild[28] = "WindowsNT %u.%u (Build: %u)";

// GLOBAL: LEMBALL 0x004a060c
char g_szWin32sBuild[28] = "Win32s %u.%u (Build: %u)";

// GLOBAL: LEMBALL 0x004a0628
char g_szWindows95Build[16] = "Windows95 %u.%u";

// GLOBAL: LEMBALL 0x004a0638
char g_szAboutSystemFormat[152] =
	"Operating System : %s\nBuild Number : %d\nViSOS Build Number : %d\nPhysical Memory Size : %ld bytes\nPercent Used : %d%%\nAvail Memory Free : %ld bytes\n";

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winver.h>

// GLOBAL: LEMBALL 0x004a06cc
char g_szAboutFontFace[4] = "";

// GLOBAL: LEMBALL 0x004a06d0
char g_szStringFileInfoPrefix[28] = "\\StringFileInfo\\080904e4\\";

// GLOBAL: LEMBALL 0x004a7b88
MEMORYSTATUS g_memoryStatus;

// GLOBAL: LEMBALL 0x004a7ba8
HFONT g_hAboutFont = 0;

// GLOBAL: LEMBALL 0x004a7bb0
char g_szAboutSystemInfo[0x400];

// GLOBAL: LEMBALL 0x0049e798
char g_szAboutBox[12] = "AboutBox";

// GLOBAL: LEMBALL 0x0049e7d8
char g_szCouldntHelpYa[20] = "Couldn't help ya!\n";

// GLOBAL: LEMBALL 0x0049e7a4
char g_szLemballHelpFile[20] = "lemball\\lemball.hlp";

// GLOBAL: LEMBALL 0x0049e7cc
char g_szHelpContentsKey[12] = "CONTENTS";

// FUNCTION: LEMBALL 0x00455f00
void CenterWindowOnParent(void* p_window, void* p_parent)
{
	RECT windowRect;
	RECT parentRect;
	HDC dc;
	int width;
	int height;
	int parentWidth;
	int parentHeight;
	int screenWidth;
	int screenHeight;
	int x;
	int y;

	GetWindowRect((HWND) p_window, &windowRect);
	width = windowRect.right - windowRect.left;
	height = windowRect.bottom - windowRect.top;
	GetWindowRect((HWND) p_parent, &parentRect);
	parentWidth = parentRect.right - parentRect.left;
	parentHeight = parentRect.bottom - parentRect.top;
	dc = GetDC((HWND) p_window);
	screenWidth = GetDeviceCaps(dc, 8);
	screenHeight = GetDeviceCaps(dc, 10);
	ReleaseDC((HWND) p_window, dc);
	x = parentRect.left + (parentWidth - width) / 2;
	if (x < 0) {
		x = 0;
	}
	else if (screenWidth < width + x) {
		x = screenWidth - width;
	}
	y = (parentHeight - height) / 2 + parentRect.top;
	if (y < 0) {
		y = 0;
	}
	else if (screenHeight < height + y) {
		y = screenHeight - height;
	}
	SetWindowPos((HWND) p_window, 0, x, y, 0, 0, 5);
}

#include <string.h>
#pragma intrinsic(strcat)

// FUNCTION: LEMBALL 0x00455ff0
char* BuildAboutSystemInfo()
{
	unsigned long version = GetVersion();
	unsigned short versionWord = (unsigned short) version;
	char osText[256];

	if (version < 0x80000000) {
		wsprintfA(osText, g_szWindowsNtBuild, (unsigned int) (unsigned char) versionWord, (unsigned int) (unsigned char) (versionWord >> 8), (unsigned int) HIWORD(version));
	}
	else if ((unsigned char) versionWord < 4) {
		wsprintfA(osText, g_szWin32sBuild, (unsigned int) (unsigned char) versionWord, (unsigned int) (unsigned char) (versionWord >> 8), (unsigned int) HIWORD(version) & ~0x8000);
	}
	else {
		wsprintfA(osText, g_szWindows95Build, (unsigned int) (unsigned char) versionWord, (unsigned int) (unsigned char) (versionWord >> 8));
	}
	wsprintfA(g_szAboutSystemInfo,
			  g_szAboutSystemFormat,
			  osText,
			  0x33,
			  g_nVisosBuildNumber,
			  g_memoryStatus.dwTotalPhys,
			  g_memoryStatus.dwMemoryLoad,
			  g_memoryStatus.dwAvailPhys);
	strcat(g_szAboutSystemInfo, g_pSoundManager->BuildDriverInfo());
	return g_szAboutSystemInfo;
}

// FUNCTION: LEMBALL 0x00456100
int __stdcall AboutDialogProc(void* p_dlg, unsigned int p_msg, unsigned int p_wParam, long p_lParam)
{
	int controlId;
	char* systemInfo;
	HWND control;
	unsigned long handle;
	unsigned int versionSize;
	void* versionData;
	char modulePath[256];
	char itemText[256];
	char queryPath[1024];
	unsigned int queryLen;
	char* queryValue;
	int queryOk;
	unsigned int prefixLen;

	(void) p_lParam;
	switch (p_msg) {
	case WM_INITDIALOG:
		controlId = 0x400;
		g_memoryStatus.dwLength = 0x20;
		g_memoryStatus.dwTotalPhys = 0x400;
		GlobalMemoryStatus(&g_memoryStatus);
		g_hAboutFont = CreateFontA(0xe, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x22, g_szAboutFontFace);
		CenterWindowOnParent(p_dlg, GetWindow((HWND) p_dlg, 4));
		systemInfo = BuildAboutSystemInfo();
		SetDlgItemTextA((HWND) p_dlg, 0x11b, systemInfo);
		control = GetDlgItem((HWND) p_dlg, 0x11b);
		SendMessageA(control, 0x30, (unsigned int) g_hAboutFont, 1);
		GetModuleFileNameA((HINSTANCE) g_pApplicationInstance, modulePath, 0x100);
		versionSize = GetFileVersionInfoSizeA(modulePath, &handle);
		if (versionSize != 0) {
			versionData = operator new(versionSize);
			GetFileVersionInfoA(modulePath, handle, versionSize, versionData);
			lstrcpyA(queryPath, g_szStringFileInfoPrefix);
			prefixLen = (unsigned int) (unsigned short) lstrlenA(queryPath);
			do {
				GetDlgItemTextA((HWND) p_dlg, controlId, itemText, 0x100);
				queryPath[prefixLen] = 0;
				lstrcatA(queryPath, itemText);
				queryLen = 0;
				queryValue = 0;
				queryOk = VerQueryValueA(versionData, queryPath, (void**) &queryValue, &queryLen);
				if (queryOk != 0 && queryLen != 0 && queryValue != 0) {
					lstrcpyA(itemText, queryValue);
					SetDlgItemTextA((HWND) p_dlg, controlId, itemText);
					control = GetDlgItem((HWND) p_dlg, controlId);
					SendMessageA(control, 0x30, (unsigned int) g_hAboutFont, 1);
				}
				controlId = controlId + 1;
			} while (controlId <= 0x404);
			operator delete(versionData);
		}
		return 1;
	case WM_COMMAND:
		if ((short) p_wParam == 1 || (short) p_wParam == 2) {
			EndDialog((HWND) p_dlg, 1);
			DeleteObject(g_hAboutFont);
			return 1;
		}
		break;
	}
	return 0;
}
