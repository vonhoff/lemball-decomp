#include "TargetTextWindow.h"

#include "../Foundation/VsDebug.h"

#include <stdlib.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// GLOBAL: LEMBALL 0x004a2c40
static char g_unableToAllocateTextCopy[] = "Unable to allocate memory for string copy";

// GLOBAL: LEMBALL 0x004a2c6c
static char g_textWindowInfo[] = "INFO";

// GLOBAL: LEMBALL 0x004a2c74
static char g_unableToInvalidateTextLines[] = "RedrawLines : InvalidateRect==FALSE";

// FUNCTION: LEMBALL 0x00473d90
void TargetTextWindow::PostAllocatedTextControlString(const char* p_text, unsigned int p_color)
{
	EnterCritical();
	if (g_nTargetTextWindowActive != 0) {
		char* copy = (char*) malloc(strlen(p_text) + 1);
		if (copy == 0) {
			FatalWin32Error(g_unableToAllocateTextCopy);
		}
		strcpy(copy, p_text);
		if (m_windowHandle != 0) {
			PostMessageA((HWND) m_windowHandle, 0x420, (unsigned int) copy, p_color);
		}
		LeaveCritical();
	}
}

// FUNCTION: LEMBALL 0x00473eb0
static void GetWindowClientScreenRect(HWND p_window, RECT* p_rect)
{
	POINT origin;
	origin.x = 0;
	origin.y = 0;
	ClientToScreen(p_window, &origin);
	GetClientRect(p_window, p_rect);
	p_rect->top += origin.y;
	p_rect->bottom += origin.y;
	p_rect->left += origin.x;
	p_rect->right += origin.x;
}

// FUNCTION: LEMBALL 0x00473f00
int TargetTextWindow::PointToLine(int p_x, int p_y)
{
	RECT rect;
	EnterCritical();
	GetWindowClientScreenRect((HWND) m_windowHandle, &rect);
	int height = rect.bottom - rect.top;
	int row = p_y - rect.top;
	if (row < 0) {
		row = -1;
	}
	else if (height < row) {
		row = m_visibleRowsCeiling + 1;
	}
	else {
		row /= m_lineHeight;
	}
	int line = m_topLine + row;
	LeaveCritical();
	return line;
}

// FUNCTION: LEMBALL 0x00473f60
void TargetTextWindow::RedrawAll()
{
	EnterCritical();
	InvalidateRect((HWND) m_windowHandle, 0, 0);
	LeaveCritical();
}

// FUNCTION: LEMBALL 0x00473f80
void TargetTextWindow::RedrawLines(int p_firstLine, int p_lineCount)
{
	RECT rect;
	EnterCritical();
	int row = p_firstLine - m_topLine;
	if (row >= 0 && row < m_visibleRowsCeiling) {
		rect.left = 0;
		rect.right = m_clientWidth;
		rect.top = row * m_lineHeight;
		rect.bottom = m_lineHeight * p_lineCount + rect.top;
		if (InvalidateRect((HWND) m_windowHandle, &rect, 0) == 0) {
			MessageBoxA(0, g_unableToInvalidateTextLines, g_textWindowInfo, 0);
		}
	}
	LeaveCritical();
}

// FUNCTION: LEMBALL 0x00474000
int TargetTextWindow::UpdateVisibleRows()
{
	RECT rect;
	EnterCritical();
	GetClientRect((HWND) m_windowHandle, &rect);
	int lineHeight;
	int height = rect.bottom - rect.top;
	lineHeight = m_lineHeight;
	m_visibleRows = height / lineHeight;
	m_visibleRowsCeiling = (lineHeight + height - 1) / lineHeight;
	LeaveCritical();
	return m_visibleRowsCeiling;
}

// FUNCTION: LEMBALL 0x00474050
int TargetTextWindow::UpdateClientWidth()
{
	RECT rect;
	EnterCritical();
	GetClientRect((HWND) m_windowHandle, &rect);
	m_clientWidth = rect.right - rect.left;
	LeaveCritical();
	return m_clientWidth;
}

// FUNCTION: LEMBALL 0x00474090
void TargetTextWindow::ResizeToWholeRows(int p_clientWidth, int p_clientHeight, undefined4 p_arg2)
{
	RECT rect;
	EnterCritical();
	int lineHeight = m_lineHeight;
	int remainder = p_clientHeight % lineHeight;
	if (remainder != 0) {
		p_clientHeight -= remainder;
		p_clientHeight += lineHeight;
		rect.left = 0;
		rect.top = 0;
		rect.right = GetSystemMetrics(2) + p_clientWidth;
		rect.bottom = p_clientHeight;
		AdjustWindowRect(&rect, GetWindowLongA((HWND) m_windowHandle, -16), 0);
		SetWindowPos((HWND) m_windowHandle, 0, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 6);
	}
	else {
		UpdateVisibleRows();
		UpdateClientWidth();
	}
	LeaveCritical();
}

// GLOBAL: LEMBALL 0x004a44d8 SYMBOL
// __locktable

// GLOBAL: LEMBALL 0x004a5038 SYMBOL
// __newmode

// GLOBAL: LEMBALL 0x004aa494 SYMBOL
// ?_pnhHeap@@3P6AHI@ZA

// GLOBAL: LEMBALL 0x004ab7e0 SYMBOL
// __crtheap

// GLOBAL: LEMBALL 0x004a29f0
TargetTextWindow* g_pDebugWindow = 0;

// GLOBAL: LEMBALL 0x004a2b68
int g_nTargetTextWindowClassRegistered = 0;

// GLOBAL: LEMBALL 0x004a2b7c
int g_nTargetTextWindowActive = 0;

// GLOBAL: LEMBALL 0x004a2b8c
int g_nTargetTextWindowCreated = 0;
