#include "CCursor.h"

#include "Application/CDemo.h"
#include "Engine/Resources/Manifest.h"
#include "Engine/Math/CVSPoint.h"
#include "Platform/Windows/Input/CBaseCursor.h"

#include <stddef.h>

extern "C" __declspec(dllimport) void* __stdcall LoadCursorA(void* p_instance, const char* p_name);
extern "C" __declspec(dllimport) int __stdcall GetCursorPos(void* p_point);
extern "C" __declspec(dllimport) void* __stdcall SetCursor(void* p_cursor);
extern "C" __declspec(dllimport) int __stdcall ShowCursor(int p_show);

// GLOBAL: LEMBALL 0x004a9bf4
CCursor* g_pCursor = NULL;

// GLOBAL: LEMBALL 0x0049ee10
unsigned int g_cursorResourceIds[4] = {0, RES_CURSORS_HAND, RES_CURSORS_PAW_CURSOR, 0};

// GLOBAL: LEMBALL 0x0049ee20
unsigned int g_cursorDisplayInited = 0;

// FUNCTION: LEMBALL 0x0043a720
void CursorChangeType(eCursorDisplayType p_cursorType, int p_frame)
{
	CCursor* cursor;

	if ((unsigned int) p_cursorType > 3) {
		return;
	}
	switch (p_cursorType) {
	case CURSOR_DISPLAY_NONE:
		g_pCursor->SetActive(0);
		g_pCursor->SetMainID(g_cursorResourceIds[p_cursorType]);
		break;
	case CURSOR_DISPLAY_HAND:
		if (g_pDemo != NULL && g_pDemo->m_demoMode != 0) {
			p_frame = 0;
		}
		g_pCursor->SetMainID(g_cursorResourceIds[p_cursorType], p_frame);
		if (g_cursorDisplayInited == 0) {
			g_pCursor->m_mouseInput = 1;
			g_cursorDisplayInited = 1;
		}
		g_pCursor->SetActive(1);
		cursor = g_pCursor;
		g_pCursor->m_hotspot.m_x = 3;
		cursor->m_hotspot.m_y = 3;
		break;
	case CURSOR_DISPLAY_PAW:
	case CURSOR_DISPLAY_HIDDEN:
		g_pCursor->SetMainID(g_cursorResourceIds[p_cursorType]);
		if (g_cursorDisplayInited == 0) {
			g_pCursor->m_mouseInput = 1;
			cursor = g_pCursor;
			g_pCursor->m_hotspot.m_x = 3;
			cursor->m_hotspot.m_y = 3;
			g_cursorDisplayInited = 1;
		}
		g_pCursor->SetActive(1);
		break;
	}
}

// FUNCTION: LEMBALL 0x00474b50
void CCursor::InitialiseSystemCursor()
{
	m_systemCursor = NULL;
	m_systemCursor = LoadCursorA(NULL, (char*) 0x7f00);
	RefreshPos();
}

struct CursorPos {
	int m_x;
	int m_y;
};

// FUNCTION: LEMBALL 0x00474b80
void CCursor::RefreshPos()
{
	CursorPos point;

	GetCursorPos(&point);
	m_position.m_x = (short) point.m_x;
	m_position.m_y = (short) point.m_y;
}

// FUNCTION: LEMBALL 0x00474bb0
CCursor::~CCursor()
{
	SetCursor(m_systemCursor);
	RestoreSystemCursor();
}

// FUNCTION: LEMBALL 0x00474be0
void CCursor::KillSystemCursor()
{
	SetCursor(NULL);
	ShowCursor(0);
	m_systemCursorVisible = 0;
}

// FUNCTION: LEMBALL 0x00474c00
void CCursor::RestoreSystemCursor()
{
	ShowCursor(1);
	m_systemCursorVisible = 1;
}
