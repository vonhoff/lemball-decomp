#include "CCursor.h"

#include "../../Control/Game/CDemo.h"
#include "../Resources/Manifest.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Graphics/CBaseCursor.h"

extern "C" __declspec(dllimport) void* __stdcall LoadCursorA(void* p_instance, const char* p_name);
extern "C" __declspec(dllimport) int __stdcall GetCursorPos(void* p_point);
extern "C" __declspec(dllimport) void* __stdcall SetCursor(void* p_cursor);
extern "C" __declspec(dllimport) int __stdcall ShowCursor(int p_show);

// GLOBAL: LEMBALL 0x004a9bf4
CCursor* g_pCursor = 0;

// GLOBAL: LEMBALL 0x0049ee10
unsigned int g_cursorResourceIds[4] = {0, RES_CURSORS_HAND, RES_CURSORS_PAW_CURSOR, 0};

// GLOBAL: LEMBALL 0x0049ee20
unsigned int g_cursorDisplayInited = 0;

// FUNCTION: LEMBALL 0x0043a720
void CursorChangeType(eCursorDisplayType p_arg0, int p_arg1)
{
	CCursor* cursor;

	if ((unsigned int) p_arg0 > 3) {
		return;
	}
	switch (p_arg0) {
	case CURSOR_DISPLAY_NONE:
		g_pCursor->SetActive(0);
		g_pCursor->SetMainId(g_cursorResourceIds[p_arg0]);
		break;
	case CURSOR_DISPLAY_HAND:
		if (g_pDemo != 0 && g_pDemo->m_demoMode != 0) {
			p_arg1 = 0;
		}
		g_pCursor->SetMainId(g_cursorResourceIds[p_arg0], p_arg1);
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
	case 3:
		g_pCursor->SetMainId(g_cursorResourceIds[p_arg0]);
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
	m_systemCursor = 0;
	m_systemCursor = LoadCursorA(0, (char*) 0x7f00);
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
	SetCursor(0);
	ShowCursor(0);
	m_systemCursorVisible = 0;
}

// FUNCTION: LEMBALL 0x00474c00
void CCursor::RestoreSystemCursor()
{
	ShowCursor(1);
	m_systemCursorVisible = 1;
}
