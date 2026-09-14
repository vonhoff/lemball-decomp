#include "WindowsCursorMotionState.h"

#include "../../Map/Base/Map.h"
#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/VsPoint.h"

#include <string.h>

// FUNCTION: LEMBALL 0x00432650
WindowsCursorMotionState::~WindowsCursorMotionState()
{
}

// FUNCTION: LEMBALL 0x00432680
void WindowsCursorMotionState::SendCursorPositionMessage()
{
	int x;
	int y;
	Message message;
	message.type = 1;
	memset(&message.time, 0, 16);
	int screenX = m_fixedX >> 12;
	int screenY = m_fixedY >> 12;
	m_map->ScreenToGame(screenX, screenY, x, y);
	message.code = x;
	message.payload = (void*) y;
	m_aiQueue->Post(message);
}

// FUNCTION: LEMBALL 0x00432810
void WindowsCursorMotionState::SetCursorMotionPoint(const VsPoint& p_position)
{
	m_positionDirty = 1;
	m_horizontalActive = 0;
	m_verticalActive = 0;
	m_fixedY = p_position.m_y << 12;
	m_fixedX = p_position.m_x << 12;
}

// FUNCTION: LEMBALL 0x00432840
VsPoint WindowsCursorMotionState::GetCursorMotionPoint()
{
	return VsPoint((short) (m_fixedX >> 12), (short) (m_fixedY >> 12));
}

// FUNCTION: LEMBALL 0x00432920
void WindowsCursorMotionState::SetCursorMotionAuxValues(unsigned int p_aux0, unsigned int p_aux1)
{
	m_aux0 = p_aux0;
	m_aux1 = p_aux1;
}
