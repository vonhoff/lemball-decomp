#include "TargetNetworkWindow.h"

#define WIN32_LEAN_AND_MEAN
#include "../../Platform/Windows/Entry.h"
#include "../Foundation/VsDebug.h"
#include "../Foundation/VsOStream.h"

#include <windows.h>

extern unsigned int g_unk0x4a1cc4;

static LRESULT CALLBACK TargetNetworkWindow_WindowProc(HWND p_window, UINT p_message, WPARAM p_wParam, LPARAM p_lParam);

// FUNCTION: LEMBALL 0x0047c050
TargetNetworkWindow::TargetNetworkWindow(const char* p_className, int* p_registered)
	: m_firstMessage(0), m_lastMessage(0)
{
	WNDCLASSA windowClass;

	if (*p_registered == 0) {
		windowClass.style = 0;
		windowClass.lpfnWndProc = TargetNetworkWindow_WindowProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 4;
		windowClass.hInstance = (HINSTANCE) g_pApplicationInstance;
		windowClass.hIcon = 0;
		windowClass.hCursor = 0;
		windowClass.hbrBackground = 0;
		windowClass.lpszMenuName = 0;
		windowClass.lpszClassName = p_className;
		if (RegisterClassA(&windowClass) == 0) {
			FatalWin32Error("Unable to register base window class");
		}
		*p_registered = 1;
	}

	m_windowHandle = CreateWindowExA(0,
									 p_className,
									 "Invisible Window",
									 0,
									 CW_USEDEFAULT,
									 CW_USEDEFAULT,
									 CW_USEDEFAULT,
									 CW_USEDEFAULT,
									 0,
									 0,
									 (HINSTANCE) g_pApplicationInstance,
									 this);
	if (m_windowHandle == 0) {
		*g_pErrorOutput << "Failed to create invisible window - Error code: " << (unsigned long) GetLastError() << "\n";
	}
}

// FUNCTION: LEMBALL 0x0047c140
TargetNetworkWindow::~TargetNetworkWindow()
{
	DestroyWindow((HWND) m_windowHandle);
}

// FUNCTION: LEMBALL 0x0047c160
static LRESULT CALLBACK TargetNetworkWindow_WindowProc(HWND p_window, UINT p_message, WPARAM p_wParam, LPARAM p_lParam)
{
	TargetNetworkWindow* window;
	long result;

	if (g_unk0x4a1cc4 != 0) {
		return DefWindowProcA(p_window, p_message, p_wParam, p_lParam);
	}

	window = (TargetNetworkWindow*) GetWindowLongA(p_window, GWL_USERDATA);
	if (p_message != WM_CREATE) {
		if (window != 0 && (window->m_lastMessage == 0 ||
							(window->m_firstMessage <= p_message && p_message <= window->m_lastMessage))) {
			result = window->Process(p_message, p_wParam, p_lParam);
			if (result != -1) {
				return result;
			}
		}
		return DefWindowProcA(p_window, p_message, p_wParam, p_lParam);
	}
	SetWindowLongA(p_window, GWL_USERDATA, (LONG) ((CREATESTRUCTA*) p_lParam)->lpCreateParams);
	return 0;
}

// GLOBAL: LEMBALL 0x004a1cc4
unsigned int g_unk0x4a1cc4 = 0;
