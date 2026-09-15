#include "../../Visos/Foundation/Vector.h"

// Descriptive name; the original class name is unknown. The endpoint setter
// is called with GameObject + 0x78 by StartMoving and Ball::StartMovement.
class MovementInterpolation {
public:
	void SetEndpoints(Vector p_start, Vector p_end);
	MovementInterpolation& operator=(const MovementInterpolation& p_other);

private:
	int m_startX; // 0x00
	int m_startY; // 0x04
	int m_deltaX; // 0x08
	int m_deltaY; // 0x0c
};

// FUNCTION: LEMBALL 0x00417b00
void MovementInterpolation::SetEndpoints(Vector p_start, Vector p_end)
{
	m_deltaX = p_end.m_xFixed - p_start.m_xFixed;
	m_deltaY = p_end.m_yFixed - p_start.m_yFixed;
	m_startX = p_start.m_xFixed;
	m_startY = p_start.m_yFixed;
}

// FUNCTION: LEMBALL 0x004267a0
MovementInterpolation& MovementInterpolation::operator=(const MovementInterpolation& p_other)
{
	m_startX = p_other.m_startX;
	m_startY = p_other.m_startY;
	m_deltaX = p_other.m_deltaX;
	m_deltaY = p_other.m_deltaY;
	return *this;
}
