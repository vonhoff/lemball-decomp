#ifndef LEMBALL_AI_BASE_MOVE3D_H
#define LEMBALL_AI_BASE_MOVE3D_H

#include "../../Common.h"
#include "Pt3.h" // complete type

// SIZE 0x1c
class Move3d {
public:
	inline Move3d()
	{
		m_start.m_x = 0;
		m_start.m_y = 0;
		m_start.m_z = 0;
		m_velocity.m_x = DEBUG_SENTINEL;
		m_velocity.m_y = DEBUG_SENTINEL;
		m_velocity.m_z = DEBUG_SENTINEL;
	}
	void Position(Pt3& p_position, int p_time);
	void Set(Pt3& p_start, Pt3& p_end, int p_startTime, int p_speed);

private:
	int m_startTime; // 0x00
	Pt3 m_start;     // 0x04
	Pt3 m_velocity;  // 0x10
};

#endif
