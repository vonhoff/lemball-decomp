#include "TargetTextWindow.h"

#include "../Foundation/VsDebug.h"
#include "TargetTextLine.h"
#include "TargetTextLineBuffer.h"

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

// FUNCTION: LEMBALL 0x00474290
void TargetTextWindow::Scroll(int p_scrollCode, int p_thumbPos)
{
	EnterCritical();
	switch (p_scrollCode) {
	case 0:
		m_topLine--;
		break;
	case 1:
		m_topLine++;
		break;
	case 2:
		m_topLine -= m_visibleRowsCeiling;
		break;
	case 3:
		m_topLine += m_visibleRowsCeiling;
		break;
	case 4:
	case 5:
		m_topLine = p_thumbPos;
		break;
	case 6:
		m_topLine = 0;
		break;
	case 7:
		m_topLine = m_lineCount;
		break;
	}
	if (m_topLine >= m_lineCount) {
		m_topLine = m_lineCount - 1;
	}
	if (m_topLine < 0) {
		m_topLine = 0;
	}
	SetScrollPos((HWND) m_windowHandle, 1, m_topLine, 1);
	RedrawAll();
	LeaveCritical();
}

// FUNCTION: LEMBALL 0x00474340
void TargetTextWindow::BeginSelection(int p_x, int p_y, undefined4 p_arg2)
{
	EnterCritical();
	m_dragLine = PointToLine(p_x, p_y);
	if (m_lineCount <= m_dragLine) {
		LeaveCritical();
		return;
	}
	SetCapture((HWND) m_windowHandle);
	m_selecting = 1;
	SetSelectionHighlight(0);
	m_selectionEnd = m_dragLine;
	m_selectionStart = m_dragLine;
	m_selectionAnchor = m_dragLine;
	SetSelectionHighlight(1);
	LeaveCritical();
}

// FUNCTION: LEMBALL 0x004743b0
void TargetTextWindow::EndSelection(undefined4 p_arg0, undefined4 p_arg1, undefined4 p_arg2)
{
	EnterCritical();
	if (m_selecting != 0) {
		ReleaseCapture();
		m_selecting = 0;
	}
	LeaveCritical();
}

// FUNCTION: LEMBALL 0x004743e0
void TargetTextWindow::SetSelectionHighlight(int p_selected)
{
	EnterCritical();
	for (int line = m_selectionStart; line <= m_selectionEnd; line++) {
		m_lineBuffer->m_lines[line].m_selected = p_selected;
		RedrawLines(line, 1);
	}
	LeaveCritical();
}

// FUNCTION: LEMBALL 0x00474430
void TargetTextWindow::EnsureLineVisible(int p_line)
{
	EnterCritical();
	if (p_line < m_topLine) {
		m_topLine = p_line;
		SetScrollPos((HWND) m_windowHandle, 1, p_line, 1);
		RedrawAll();
		LeaveCritical();
		return;
	}
	if (p_line >= m_topLine + m_visibleRows) {
		m_topLine = p_line - m_visibleRows + 1;
		SetScrollPos((HWND) m_windowHandle, 1, m_topLine, 1);
		RedrawAll();
	}
	LeaveCritical();
}

// FUNCTION: LEMBALL 0x004744a0
void TargetTextWindow::UpdateSelection(int p_x, int p_y, undefined4 p_arg2)
{
	EnterCritical();
	if (m_selecting != 0) {
		m_dragLine = PointToLine(p_x, p_y);
		if (m_dragLine >= m_lineCount) {
			m_dragLine = m_lineCount - 1;
		}
		if (m_dragLine < 0) {
			LeaveCritical();
			return;
		}
		SetSelectionHighlight(0);
		if (m_dragLine <= m_selectionAnchor) {
			m_selectionStart = m_dragLine;
		}
		if (m_dragLine >= m_selectionAnchor) {
			m_selectionEnd = m_dragLine;
		}
		SetSelectionHighlight(1);
		EnsureLineVisible(m_dragLine);
	}
	LeaveCritical();
}

// GLOBAL: LEMBALL 0x004a2c9c
static char g_unableToAllocateSelectionText[] = "GetSelectionAsString: Unable to allocate string memory";

// GLOBAL: LEMBALL 0x004a2cd8
static char g_copyBufferInfo[] = "INFO";

// GLOBAL: LEMBALL 0x004a2ce0
static char g_unableToAllocateCopyBuffer[] = "Unable to allocate copy buffer";

// GLOBAL: LEMBALL 0x004a2d00
static char g_clipboardInfo[] = "INFO";

// GLOBAL: LEMBALL 0x004a2d08
static char g_unableToAllocateClipboard[] = "Unable to allocate clipboard";

// FUNCTION: LEMBALL 0x00474520
char* TargetTextWindow::GetSelectionText()
{
	EnterCritical();
	unsigned int length = 0;
	int line;
	for (line = m_selectionStart; line <= m_selectionEnd; line++) {
		length += strlen(m_lineBuffer->m_lines[line].m_text) + 2;
	}
	char* text = (char*) malloc(length + 1);
	if (text == 0) {
		FatalWin32Error(g_unableToAllocateSelectionText);
	}
	text[0] = 0;
	for (line = m_selectionStart; line <= m_selectionEnd; line++) {
		strcat(text, m_lineBuffer->m_lines[line].m_text);
		// STRING: LEMBALL 0x004a2cd4
		memcpy(text + strlen(text), "\r\n", 3);
	}
	LeaveCritical();
	return text;
}

// FUNCTION: LEMBALL 0x00474620
void TargetTextWindow::CopySelection()
{
	EnterCritical();
	if (m_selectionStart == -1 || m_selectionEnd == -1) {
		LeaveCritical();
		return;
	}
	char* text = GetSelectionText();
	HGLOBAL memory = GlobalAlloc(0x2002, strlen(text) + 1);
	if (memory == 0) {
		free(text);
		MessageBoxA(0, g_unableToAllocateCopyBuffer, g_copyBufferInfo, 0);
		LeaveCritical();
		return;
	}
	char* copy = (char*) GlobalLock(memory);
	strcpy(copy, text);
	GlobalUnlock(memory);
	free(text);
	if (OpenClipboard((HWND) m_windowHandle) != 0) {
		EmptyClipboard();
		SetClipboardData(1, memory);
		CloseClipboard();
	}
	else {
		MessageBoxA(0, g_unableToAllocateClipboard, g_clipboardInfo, 0);
		GlobalFree(memory);
	}
	SetSelectionHighlight(0);
	m_selectionEnd = -1;
	m_selectionStart = -1;
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
