#include "Cursor.h"

#include "../../Control/Game/Demo.h"
#include "../Resources/Manifest.h"

extern "C" __declspec(dllimport) void* __stdcall LoadCursorA(void* p_instance, const char* p_name);
extern "C" __declspec(dllimport) int __stdcall GetCursorPos(void* p_point);
extern "C" __declspec(dllimport) void* __stdcall SetCursor(void* p_cursor);
extern "C" __declspec(dllimport) int __stdcall ShowCursor(int p_show);

// GLOBAL: LEMBALL 0x004a9bf4
Cursor* g_pCursor = 0;

// GLOBAL: LEMBALL 0x0049ee10
unsigned int g_cursorResourceIds[4] = {0, RES_CURSORS_HAND, RES_CURSORS_PAW_CURSOR, 0};

// GLOBAL: LEMBALL 0x0049ee20
unsigned int g_cursorDisplayInited = 0;

// 68K 0x10b0000c CursorChangeType__F18eCursorDisplayTypei
// FUNCTION: LEMBALL 0x0043a720
void CursorChangeType(eCursorDisplayType p_arg0, int p_arg1)
{
	Cursor* cursor;
	int frame;

	if ((unsigned int) p_arg0 > 3) {
		return;
	}
	switch (p_arg0) {
	case 0:
		g_pCursor->SetActive(0);
		g_pCursor->SetMainId(g_cursorResourceIds[p_arg0]);
		break;
	case 1:
		if (g_pDemo != 0 && g_pDemo->m_demoMode != 0) {
			frame = 0;
		}
		else {
			frame = p_arg1;
		}
		g_pCursor->SetMainId(g_cursorResourceIds[p_arg0], frame);
		if (g_cursorDisplayInited == 0) {
			g_pCursor->m_mouseInput = 1;
			g_cursorDisplayInited = 1;
		}
		g_pCursor->SetActive(1);
		cursor = g_pCursor;
		g_pCursor->m_hotspot.m_x = 3;
		cursor->m_hotspot.m_y = 3;
		break;
	case 2:
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

// 68K 0x10106028 __dt__7CCursorFv
Cursor::~Cursor()
{
}

// FUNCTION: LEMBALL 0x00474b50
void Cursor::InitialiseSystemCursor()
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
void Cursor::RefreshPos()
{
	CursorPos point;

	GetCursorPos(&point);
	m_position.m_x = (short) point.m_x;
	m_position.m_y = (short) point.m_y;
}

// 68K 0x10105e2a KillSystemCursor__7CCursorFv
// FUNCTION: LEMBALL 0x00474be0
void Cursor::KillSystemCursor()
{
	SetCursor(0);
	ShowCursor(0);
	m_systemCursorVisible = 0;
}

// 68K 0x10105fa6 RestoreSystemCursor__7CCursorFv
// FUNCTION: LEMBALL 0x00474c00
void Cursor::RestoreSystemCursor()
{
	ShowCursor(1);
	m_systemCursorVisible = 1;
}
