
#include "CGraphicsState.h"

#include "../../Foundation/CString.h"
#include "../../Foundation/CVsOStream.h"
#include "../../Foundation/VsInit.h"
#include "../../Graphics/CGdi.h"
#include "../../Graphics/CPvWnd.h"
#include "../../Graphics/CSurface.h"
#include "CDirectDrawDriver.h"
#include "CDisplayDibDriver.h"
#include "CGdiDriver.h"
#include "CPlanarDibDriver.h"

#include <new.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include "Visos/Foundation/CVsSize.h"
#include "Visos/Graphics/CPvGWnd.h"
#include "Visos/Graphics/CWnd.h"
#include "Visos/Target/Graphics/CGraphicsDriver.h"

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
bool CGraphicsState::SelectDriver(int p_driverMode)
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
		g_pTargetGraphicsDriver = new CGdiDriver();
		break;
	case 2:
		storage = operator new(sizeof(CDisplayDibDriver));
		if (storage != 0) {
			CVsSize size;
			size.m_width = 320;
			size.m_height = 200;
			g_pTargetGraphicsDriver = new (storage) CDisplayDibDriver(size);
		}
		else {
			g_pTargetGraphicsDriver = 0;
		}
		break;
	case 3:
		storage = operator new(sizeof(CPlanarDibDriver));
		if (storage != 0) {
			CVsSize size;
			size.m_width = 320;
			size.m_height = 240;
			g_pTargetGraphicsDriver = new (storage) CPlanarDibDriver(size);
		}
		else {
			g_pTargetGraphicsDriver = 0;
		}
		break;
	case 4:
		storage = operator new(sizeof(CDirectDrawDriver));
		if (storage != 0) {
			CVsSize size;
			size.m_width = 640;
			size.m_height = 480;
			g_pTargetGraphicsDriver = new (storage) CDirectDrawDriver(&size, 1);
		}
		else {
			g_pTargetGraphicsDriver = 0;
		}
		break;
	case 6:
		mode = 4;
		storage = operator new(sizeof(CDirectDrawDriver));
		if (storage != 0) {
			CVsSize size;
			size.m_width = 640;
			size.m_height = 480;
			g_pTargetGraphicsDriver = new (storage) CDirectDrawDriver(&size, 1);
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
		g_pTargetGraphicsDriver = new CGdiDriver();
		if (g_pTargetGraphicsDriver->m_ready == 0) {
			*g_pErrorOutput << "No valid driver available\n";
			return 0;
		}
		if (m_fallbackWarningShown == 0) {
			CString warning(g_graphicsDriverErrors[mode]);
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
void CGraphicsState::NotifyDriverChange()
{
	if (g_pWindowOwnerList != 0) {
		WindowOwnerNode* node = g_pWindowOwnerList->m_head;
		while (node != 0) {
			CWnd* window = (CWnd*) node->m_window;
			HWND nativeWindow = (HWND) window->m_nativeWindow;
			if (nativeWindow != 0) {
				if ((window->GetStyle() & 0x80000000) != 0) {
					int directScroll = 1;
					if (m_driverMode == 3) {
						directScroll = 0;
					}
					((CPvGWnd*) window)->m_gdi->m_renderTarget->m_directScroll = directScroll;
				}
				SendMessageA(nativeWindow, 0x1c, 1, 0);
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
		if (g_pTargetGraphicsDriver != 0) {
			delete g_pTargetGraphicsDriver;
		}
		SelectDriver(p_driverMode);
		NotifyDriverChange();
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00458180
bool CGraphicsState::IsFullscreenDriver()
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

// FUNCTION: LEMBALL 0x004581a0
bool CGraphicsState::IsDirectDrawDriver()
{
	switch (m_driverMode) {
	case 4:
		return 1;
	default:
		return 0;
	}
}

// FUNCTION: LEMBALL 0x004581b0
bool CGraphicsState::IsDisplayDibDriver()
{
	switch (m_driverMode) {
	case 2:
	case 3:
	case 5:
		return 1;
	default:
		return 0;
	}
}

// FUNCTION: LEMBALL 0x004581d0
void CGraphicsState::UpdateDriverSize(const CVsSize& p_size)
{
	if (g_pTargetGraphicsDriver != 0) {
		CGraphicsDriver* driver = g_pTargetGraphicsDriver;
		driver->m_screenSize.m_width = p_size.m_width;
		driver->m_screenSize.m_height = p_size.m_height;
		NotifyDriverChange();
	}
}
