#include "WindowsCursorMotionState.h"

#include "../../Visos/Foundation/VsPoint.h"

// FUNCTION: LEMBALL 0x00432840
VsPoint WindowsCursorMotionState::GetCursorMotionPoint()
{
	return VsPoint((short) (m_fixedX >> 12), (short) (m_fixedY >> 12));
}
