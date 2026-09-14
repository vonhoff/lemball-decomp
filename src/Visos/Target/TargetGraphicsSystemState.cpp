#include "TargetGraphicsSystemState.h"

#include "../Foundation/String.h"
#include "../Foundation/VsInit.h"
#include "../Foundation/VsOStream.h"
#include "../Graphics/GWnd.h"
#include "../Graphics/Gdi.h"
#include "../Graphics/PvGdiBitmap.h"
#include "../Graphics/PvWnd.h"
#include "../Graphics/VsGdi.h"
#include "TargetDirectDrawDriver.h"
#include "TargetDisplayDibDriver.h"
#include "TargetGDIDriver.h"
#include "TargetGraphicsSystemState.h"
#include "TargetPlanarDisplayDibDriver.h"
#include "TargetWinGDrawCodecState.h"

#include <new.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct IcOpen {
	unsigned int dwSize;
	unsigned int fccType;
	unsigned int fccHandler;
	unsigned int dwVersion;
	unsigned int dwFlags;
	int dwError;
};

struct IcInfo {
	unsigned int dwSize;
	unsigned int fccType;
	unsigned int fccHandler;
	unsigned int dwFlags;
	unsigned int dwVersion;
	unsigned int dwVersionICM;
	WCHAR szName[16];
	WCHAR szDescription[128];
	WCHAR szDriver[128];
};

struct IcDrawBegin {
	unsigned int dwFlags;
	void* hpal;
	void* hwnd;
	void* hdc;
	int xDst;
	int yDst;
	int dxDst;
	int dyDst;
	BITMAPINFOHEADER* lpbi;
	int xSrc;
	int ySrc;
	int dxSrc;
	int dySrc;
};

struct IcDraw {
	unsigned int dwFlags;
	void* lpFormat;
	void* lpData;
};

struct IcDrawSuggest {
	BITMAPINFOHEADER* lpbiIn;
	BITMAPINFOHEADER* lpbiSuggest;
};

unsigned int __stdcall TargetWinGDrawCodec_GetInfo(void* p_info, unsigned int p_size);

extern "C" __declspec(dllimport) int __stdcall GetSystemMetrics(int p_index);
extern "C" __declspec(dllimport) long __stdcall DefDriverProc(unsigned int p_driverId,
															  void* p_driverHandle,
															  unsigned int p_message,
															  long p_param1,
															  long p_param2);

// GLOBAL: LEMBALL 0x004a0778
static const char* g_graphicsDriverNames[] = {"NO",
											  "CDS",
											  "VGA (Full Screen 320*200)",
											  "VGA (Full Screen 320*240)",
											  "Direct Draw (Full Screen 640*480)",
											  "Direct Draw (Full Screen 320*200)",
											  "Direct Draw (Windowed 640*480)",
											  "Direct Draw (Windowed 320*200)",
											  "Auto Select",
											  0};

// GLOBAL: LEMBALL 0x004a07a0
static const char* g_graphicsDriverErrors[] = {
	"None",
	"Defaulting to normal 640*480 mode (using CreateDIBSection)",
	"Unable to find DispDib32 Libraries for full screen 320*200 mode (dspdib16.dll & dspdib32.dll) - please reinstall",
	"Unable to find DispDib32 Libraries for full screen 320*240 mode (dspdib16.dll & dspdib32.dll) - please reinstall",
	"Unable to find Direct Draw libraries for full screen 640*480 mode (ddraw.dll) - please reinstall the DirectX "
	"libraries",
	"Unable to find Direct Draw libraries for full screen 320*200 mode (ddraw.dll) - please reinstall the DirectX "
	"libraries"};

// FUNCTION: LEMBALL 0x00457e10
bool TargetGraphicsSystemState::SelectDriver(int p_driverMode)
{
	int mode = p_driverMode;
	void* storage;
	if (p_driverMode < 9) {
		*g_pDebugOutput << "Initialising graphics device driver: " << g_graphicsDriverNames[p_driverMode] << "...\n";
	}
	if (p_driverMode == 8) {
		if (g_nGraphicsDriverGdk != 0) {
			mode = g_nFullscreen != 0 ? 4 : 6;
		}
		else {
			mode = g_nFullscreen != 0 ? 3 : 1;
		}
	}
	switch (mode) {
	case 1:
		g_pTargetGraphicsDriver = new TargetGDIDriver();
		break;
	case 2:
		storage = operator new(sizeof(TargetDisplayDibDriver));
		if (storage != 0) {
			VsSize size;
			size.m_width = 320;
			size.m_height = 200;
			g_pTargetGraphicsDriver = new (storage) TargetDisplayDibDriver(size);
		}
		else {
			g_pTargetGraphicsDriver = 0;
		}
		break;
	case 3:
		storage = operator new(sizeof(TargetPlanarDisplayDibDriver));
		if (storage != 0) {
			VsSize size;
			size.m_width = 320;
			size.m_height = 240;
			g_pTargetGraphicsDriver = new (storage) TargetPlanarDisplayDibDriver(size);
		}
		else {
			g_pTargetGraphicsDriver = 0;
		}
		break;
	case 4:
		storage = operator new(sizeof(TargetDirectDrawDriver));
		if (storage != 0) {
			VsSize size;
			size.m_width = 640;
			size.m_height = 480;
			g_pTargetGraphicsDriver = new (storage) TargetDirectDrawDriver(&size, 1);
		}
		else {
			g_pTargetGraphicsDriver = 0;
		}
		break;
	case 6:
		mode = 4;
		storage = operator new(sizeof(TargetDirectDrawDriver));
		if (storage != 0) {
			VsSize size;
			size.m_width = 640;
			size.m_height = 480;
			g_pTargetGraphicsDriver = new (storage) TargetDirectDrawDriver(&size, 1);
		}
		else {
			g_pTargetGraphicsDriver = 0;
		}
		break;
	default:
		*g_pErrorOutput << "No valid driver selected to initialise\n";
		return 0;
	}
	if (g_pTargetGraphicsDriver->m_ready == 0) {
		delete g_pTargetGraphicsDriver;
		g_pTargetGraphicsDriver = new TargetGDIDriver();
		if (g_pTargetGraphicsDriver->m_ready == 0) {
			*g_pErrorOutput << "No valid driver available\n";
			return 0;
		}
		if (m_fallbackWarningShown == 0) {
			String warning(g_graphicsDriverErrors[mode]);
			warning += ". Defaulting to normal window mode (using CreateDIBSection)";
			MessageBoxA(0, warning, "WARNING", 0x12000);
			m_fallbackWarningShown = 1;
		}
		mode = 1;
	}
	if (mode != p_driverMode) {
		*g_pDebugOutput << "[ Auto selected: " << g_graphicsDriverNames[mode] << " ]\n";
	}
	m_driverMode = mode;
	return 1;
}

// FUNCTION: LEMBALL 0x004580c0
void TargetGraphicsSystemState::NotifyWindowsOfGraphicsDriverChange()
{
	if (g_pWindowOwnerList != 0) {
		WindowOwnerNode* node = g_pWindowOwnerList->m_head;
		while (node != 0) {
			Wnd* window = (Wnd*) node->m_window;
			HWND nativeWindow = (HWND) window->m_nativeWindow;
			if (nativeWindow != 0) {
				if ((window->GetStyle() & 0x80000000) != 0) {
					int directScroll = 1;
					if (m_driverMode == 3) {
						directScroll = 0;
					}
					((PvGWnd*) window)->m_gdi->m_renderTarget->m_directScroll = directScroll;
				}
				SendMessageA(nativeWindow, 0x1c, 1, 0);
				window->OnDriverChange();
			}
			node = node->m_next;
		}
	}
}

// FUNCTION: LEMBALL 0x00458130
bool TargetGraphicsSystemState::ChangeDriver(int p_driverMode)
{
	if (m_driverMode != p_driverMode) {
		if (g_pTargetGraphicsDriver != 0) {
			delete g_pTargetGraphicsDriver;
		}
		SelectDriver(p_driverMode);
		NotifyWindowsOfGraphicsDriverChange();
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00458180
bool TargetGraphicsSystemState::IsFullscreenDriver()
{
	switch (m_driverMode) {
	case 2:
	case 3:
	case 4:
	case 5:
		return 1;
	default:
		return 0;
	}
}

// FUNCTION: LEMBALL 0x004581d0
void TargetGraphicsSystemState::UpdateDriverSize(const VsSize& p_size)
{
	if (g_pTargetGraphicsDriver != 0) {
		TargetGraphicsDriver* driver = g_pTargetGraphicsDriver;
		driver->m_screenSize.m_width = p_size.m_width;
		driver->m_screenSize.m_height = p_size.m_height;
		NotifyWindowsOfGraphicsDriverChange();
	}
}
