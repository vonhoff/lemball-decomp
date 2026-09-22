#include "CAiCursor.h"

#include "../../Visos/Foundation/CBaseQueue.h"
#include "CAi.h"
#include "Visos/Foundation/Message.h"

// FUNCTION: LEMBALL 0x00414da0
CAiCursor::CAiCursor(CAi* p_ai, int p_maximumX, int p_maximumY)
{
	m_ai = p_ai;
	m_maximumX = p_maximumX;
	m_maximumY = p_maximumY;
	m_queue = p_ai->m_aiQueue;
	m_queue->Attach(this, 0);
}

// FUNCTION: LEMBALL 0x00414de0
CAiCursor::~CAiCursor()
{
	m_queue->Detach(this, 0);
}

// FUNCTION: LEMBALL 0x00414e00
void CAiCursor::SetCursorXy(int p_x, int p_y)
{
	m_cursorX = p_x;
	m_cursorY = p_y;
	CheckAndClipCursorBounds();
}

// FUNCTION: LEMBALL 0x00414e20
void CAiCursor::CheckAndClipCursorBounds()
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
	cursorY = m_cursorY;
	maximumY = m_maximumY;
	if (cursorY > maximumY) {
		m_cursorY = maximumY;
	}
	else if (cursorY < 0) {
		m_cursorY = 0;
	}
}

// FUNCTION: LEMBALL 0x00414e60
void CAiCursor::GetCursorSurfaceCoordinates(int& p_x, int& p_y)
{
	p_x = m_cursorX;
	p_y = m_cursorY;
}

// FUNCTION: LEMBALL 0x00414e80
int CAiCursor::ProcessMsg(Message* p_message)
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
