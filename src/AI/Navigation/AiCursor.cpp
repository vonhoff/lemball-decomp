#include "AiCursor.h"

#include "../../Visos/Foundation/BaseQueue.h"
#include "Ai.h"

// 68K 0x10606068 __ct__9CAICursorFP3CAIii
// FUNCTION: LEMBALL 0x00414da0
AiCursor::AiCursor(Ai* p_arg0, int p_arg1, int p_arg2)
{
	m_ai = p_arg0;
	m_maximumX = p_arg1;
	m_maximumY = p_arg2;
	m_queue = p_arg0->m_aiQueue;
	m_queue->Attach(this, 0);
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
	int cursorX;
	int maximumX;
	maximumX = m_maximumX;
	cursorX = m_cursorX;

	if (maximumX < cursorX) {
		m_cursorX = maximumX;
	}
	else if (cursorX < 0) {
		m_cursorX = 0;
	}
	int maximumY;
	int cursorY;
	maximumY = m_maximumY;
	cursorY = m_cursorY;
	if (maximumY < cursorY) {
		m_cursorY = maximumY;
	}
	else if (cursorY < 0) {
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
	switch (p_message->type) {
	case 1:
		SetCursorXy(p_message->code, (int) p_message->payload);
		return 1;
	default:
		m_processedCount++;
		return 0;
	}
}

// 68K 0x106060dc __dt__9CAICursorFv
AiCursor::~AiCursor()
{
}
