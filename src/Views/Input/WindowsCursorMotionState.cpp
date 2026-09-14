#include "WindowsCursorMotionState.h"

#include "../../Visos/Foundation/VsPoint.h"

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
