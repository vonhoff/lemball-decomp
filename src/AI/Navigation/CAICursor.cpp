#include "CAICursor.h"

#include "../../Visos/Foundation/CBaseQueue.h"
#include "CAI.h"
#include "Visos/Foundation/Message.h"

// FUNCTION: LEMBALL 0x00414da0
CAICursor::CAICursor(CAI* p_ai, int p_maximumX, int p_maximumY)
{
	m_ai = p_ai;
	m_maximumX = p_maximumX;
	m_maximumY = p_maximumY;
	m_queue = p_ai->m_aiQueue;
	m_queue->Attach(this, 0);
}

// FUNCTION: LEMBALL 0x00414de0
CAICursor::~CAICursor()
{
	m_queue->Detach(this, 0);
}

// FUNCTION: LEMBALL 0x00414e00
void CAICursor::SetCursorXY(int p_x, int p_y)
{
	m_cursorX = p_x;
	m_cursorY = p_y;
	CheckAndClipCursorBounds();
}

// FUNCTION: LEMBALL 0x00414e20
void CAICursor::CheckAndClipCursorBounds()
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
void CAICursor::GetCursorSurfaceCoordinates(int& p_x, int& p_y)
{
	p_x = m_cursorX;
	p_y = m_cursorY;
}

// FUNCTION: LEMBALL 0x00414e80
int CAICursor::ProcessMsg(Message* p_message)
{
	switch (p_message->type) {
	case 1:
		SetCursorXY(p_message->code, (int) p_message->payload);
		return 1;
	default:
		m_processedCount++;
		return 0;
	}
}
