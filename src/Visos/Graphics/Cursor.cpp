#include "Cursor.h"

extern "C" __declspec(dllimport) void* __stdcall LoadCursorA(void* p_instance, const char* p_name);
extern "C" __declspec(dllimport) int __stdcall GetCursorPos(void* p_point);

// GLOBAL: LEMBALL 0x004a9bf4
Cursor* g_pCursor = 0;

// 68K 0x10b0000c CursorChangeType__F18eCursorDisplayTypei
// STUB: LEMBALL 0x0043a720
void CursorChangeType(eCursorDisplayType p_arg0, int p_arg1)
{
}

// 68K 0x10106028 __dt__7CCursorFv
// SYNTHETIC: LEMBALL 0x0046dad0
// Cursor::`scalar deleting destructor'
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
// STUB: LEMBALL 0x00474be0
void Cursor::KillSystemCursor()
{
}

// 68K 0x10105fa6 RestoreSystemCursor__7CCursorFv
// STUB: LEMBALL 0x00474c00
void Cursor::RestoreSystemCursor()
{
}
