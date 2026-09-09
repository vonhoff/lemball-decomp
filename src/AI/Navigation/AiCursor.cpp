#include "AiCursor.h"

// 68K 0x10606068 __ct__9CAICursorFP3CAIii
// STUB: LEMBALL 0x00414da0
AiCursor::AiCursor(Ai* p_arg0, int p_arg1, int p_arg2)
{
}

// 68K 0x1060613e SetCursorXY__9CAICursorFii
// FUNCTION: LEMBALL 0x00414e00
void AiCursor::SetCursorXy(int p_x, int p_y)
{
	m_cursorX = p_x;
	m_cursorY = p_y;
	CheckAndClipCursorBounds();
}

// 68K 0x10606182 CheckAndClipCursorBounds__9CAICursorFv
// FUNCTION: LEMBALL 0x00414e20
void AiCursor::CheckAndClipCursorBounds()
{
	int cursorX = m_cursorX;
	int maximumX = m_maximumX;

	if (maximumX < cursorX) {
		m_cursorX = maximumX;
	}
	else if (cursorX < 0) {
		m_cursorX = 0;
	}
	if (m_cursorY > m_maximumY) {
		m_cursorY = m_maximumY;
	}
	else if (m_cursorY < 0) {
		m_cursorY = 0;
	}
}

// 68K 0x106061f0 GetCursorSurfaceCoordinates__9CAICursorFRiRi
// FUNCTION: LEMBALL 0x00414e60
void AiCursor::GetCursorSurfaceCoordinates(int& p_x, int& p_y)
{
	p_x = m_cursorX;
	p_y = m_cursorY;
}

// 68K 0x1060623c ProcessMsg__9CAICursorFP10tagMESSAGE
// FUNCTION: LEMBALL 0x00414e80
int AiCursor::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x106060dc __dt__9CAICursorFv
AiCursor::~AiCursor()
{
}
