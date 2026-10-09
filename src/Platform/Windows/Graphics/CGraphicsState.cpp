
#include "CGraphicsState.h"

#include "CDirectDrawDriver.h"
#include "CDisplayDibDriver.h"
#include "CGdiDriver.h"
#include "CPlanarDibDriver.h"
#include "CSurface.h"
#include "Engine/Graphics/Primitives/CGDI.h"
#include "Engine/Startup/VsInit.h"
#include "Engine/Streams/CVSOStream.h"
#include "Engine/Strings/CString.h"
#include "Platform/Windows/Windowing/CPVWnd.h"
#include "Platform/Windows/Windowing/CWnd.h"

#include <new.h>
#include <stddef.h>

#define WIN32_LEAN_AND_MEAN
#include "CGraphicsDriver.h"
#include "Engine/Math/CVSSize.h"
#include "Platform/Windows/Windowing/CPVGWnd.h"

#include <windows.h>

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
											  NULL};

// GLOBAL: LEMBALL 0x004a07a0
static char* g_graphicsDriverErrors[] = {
	"None",
	"Defaulting to normal 640*480 mode (using CreateDIBSection)",
	"Unable to find DispDib32 Libraries for full screen 320*200 mode (dspdib16.dll & dspdib32.dll) - please reinstall",
	"Unable to find DispDib32 Libraries for full screen 320*240 mode (dspdib16.dll & dspdib32.dll) - please reinstall",
	"Unable to find Direct Draw libraries for full screen 640*480 mode (ddraw.dll) - please reinstall the DirectX "
	"libraries",
	"Unable to find Direct Draw libraries for full screen 320*200 mode (ddraw.dll) - please reinstall the DirectX "
	"libraries"};

// FUNCTION: LEMBALL 0x00457e10
bool CGraphicsState::SelectDriver(int p_driverMode)
{
	int resolvedDriverMode = p_driverMode;
	void* driverStorage;
	if (p_driverMode < GFX_MODE_AUTO + 1) {
		*g_pDebugOutput << "Initialising graphics device driver: " << g_graphicsDriverNames[p_driverMode] << "...\n";
	}
	if (p_driverMode == GFX_MODE_AUTO) {
		if (g_nGraphicsDriverGdk != 0) {
			resolvedDriverMode = g_nFullscreen != 0 ? GFX_MODE_DD_FS_640X480 : GFX_MODE_DD_WIN_640X480;
		}
		else {
			resolvedDriverMode = g_nFullscreen != 0 ? GFX_MODE_VGA_320X240 : GFX_MODE_GDI;
		}
	}
	switch (resolvedDriverMode) {
	case GFX_MODE_GDI:
		g_pTargetGraphicsDriver = new CGdiDriver();
		break;
	case GFX_MODE_VGA_320X200:
		driverStorage = operator new(sizeof(CDisplayDibDriver));
		if (driverStorage != NULL) {
			CVSSize size;
			size.m_width = 320;
			size.m_height = 200;
			g_pTargetGraphicsDriver = new (driverStorage) CDisplayDibDriver(size);
		}
		else {
			g_pTargetGraphicsDriver = NULL;
		}
		break;
	case GFX_MODE_VGA_320X240:
		driverStorage = operator new(sizeof(CPlanarDibDriver));
		if (driverStorage != NULL) {
			CVSSize size;
			size.m_width = 320;
			size.m_height = 240;
			g_pTargetGraphicsDriver = new (driverStorage) CPlanarDibDriver(size);
		}
		else {
			g_pTargetGraphicsDriver = NULL;
		}
		break;
	case GFX_MODE_DD_FS_640X480: {
		g_pTargetGraphicsDriver = new CDirectDrawDriver(CVSSize(640, 480), 1);
		break;
	}
	case GFX_MODE_DD_WIN_640X480: {
		resolvedDriverMode = GFX_MODE_DD_FS_640X480;
		g_pTargetGraphicsDriver = new CDirectDrawDriver(CVSSize(640, 480), 1);
		break;
	}
	default:
		*g_pErrorOutput << "No valid driver selected to initialise\n";
		return false;
	}
	if (g_pTargetGraphicsDriver->m_ready == 0) {
		delete g_pTargetGraphicsDriver;
		g_pTargetGraphicsDriver = new CGdiDriver();
		if (g_pTargetGraphicsDriver->m_ready == 0) {
			*g_pErrorOutput << "No valid driver available\n";
			return false;
		}
		if (m_fallbackWarningShown == 0) {
			CString warning(g_graphicsDriverErrors[resolvedDriverMode]);
			warning += ". Defaulting to normal window mode (using CreateDIBSection)";
			MessageBoxA(NULL, warning, "WARNING", MB_TASKMODAL | MB_SETFOREGROUND);
			m_fallbackWarningShown = 1;
		}
		resolvedDriverMode = GFX_MODE_GDI;
	}
	if (resolvedDriverMode != p_driverMode) {
		*g_pDebugOutput << "[ Auto selected: " << g_graphicsDriverNames[resolvedDriverMode] << " ]\n";
	}
	m_driverMode = resolvedDriverMode;
	return true;
}

// FUNCTION: LEMBALL 0x004580c0
void CGraphicsState::NotifyDriverChange()
{
	if (g_pWindowOwnerList != NULL) {
		WindowOwnerNode* node = g_pWindowOwnerList->m_head;
		while (node != NULL) {
			CWnd* window = (CWnd*) node->m_window;
			HWND nativeWindow = (HWND) window->m_nativeWindow;
			if (nativeWindow != NULL) {
				if ((window->GetStyle() & WINDOW_STYLE_DIRECT_SCROLL) != 0) {
					int directScroll = 1;
					if (m_driverMode == GFX_MODE_VGA_320X240) {
						directScroll = 0;
					}
					((CPVGWnd*) window)->m_gdi->m_renderTarget->m_directScroll = directScroll;
				}
				SendMessageA(nativeWindow, WM_ACTIVATEAPP, TRUE, 0);
				window->OnDriverChange();
			}
			node = node->m_next;
		}
	}
}

// FUNCTION: LEMBALL 0x00458130
bool CGraphicsState::ChangeDriver(int p_driverMode)
{
	if (m_driverMode != p_driverMode) {
		if (g_pTargetGraphicsDriver != NULL) {
			delete g_pTargetGraphicsDriver;
		}
		SelectDriver(p_driverMode);
		NotifyDriverChange();
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x00458180
bool CGraphicsState::IsFullscreenDriver()
{
	switch (m_driverMode) {
	case GFX_MODE_VGA_320X200:
	case GFX_MODE_VGA_320X240:
	case GFX_MODE_DD_FS_640X480:
	case GFX_MODE_DD_FS_320X200:
		return true;
	default:
		return false;
	}
}

// FUNCTION: LEMBALL 0x004581a0
bool CGraphicsState::IsDirectDrawDriver()
{
	switch (m_driverMode) {
	case GFX_MODE_DD_FS_640X480:
		return true;
	default:
		return false;
	}
}

// FUNCTION: LEMBALL 0x004581b0
bool CGraphicsState::IsDisplayDibDriver()
{
	switch (m_driverMode) {
	case GFX_MODE_VGA_320X200:
	case GFX_MODE_VGA_320X240:
	case GFX_MODE_DD_FS_320X200:
		return true;
	default:
		return false;
	}
}

// FUNCTION: LEMBALL 0x004581d0
void CGraphicsState::UpdateDriverSize(const CVSSize& p_size)
{
	if (g_pTargetGraphicsDriver != NULL) {
		CGraphicsDriver* driver = g_pTargetGraphicsDriver;
		driver->m_screenSize.m_width = p_size.m_width;
		driver->m_screenSize.m_height = p_size.m_height;
		NotifyDriverChange();
	}
}
