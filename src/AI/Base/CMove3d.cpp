#include "CMove3d.h"

#include "../../Control/Game/CGame.h"
#include "../../Visos/Foundation/CVSMath.h"
#include "AI/Base/CPt3.h"

// FUNCTION: LEMBALL 0x0042a7d0
void CMove3d::Set(CPt3& p_start, CPt3& p_end, int p_startTime, int p_speed)
{
	m_start = p_start;
	m_startTime = p_startTime;

	int dx = p_end.m_x - p_start.m_x;
	int dy = p_end.m_y - p_start.m_y;
	int dz = p_end.m_z - p_start.m_z;
	while (dx > 100 || dx < -100 || dy > 100 || dy < -100 || dz > 100 || dz < -100) {
		dx /= 4;
		dy /= 4;
		dz /= 4;
	}

	int distance = dz * dz;
	distance += dy * dy;
	distance += dx * dx;
	if (distance == 0) {
		m_velocity.m_xFixed = 0;
		m_velocity.m_yFixed = 0;
		m_velocity.m_zFixed = 0;
		return;
	}

	int root = ((CVSMath*) g_pSentinel)->SqRoot(distance);
	m_velocity.m_xFixed = dx * 0x1000;
	m_velocity.m_yFixed = dy * 0x1000;
	m_velocity.m_zFixed = dz * 0x1000;
	m_velocity.m_xFixed = p_speed * m_velocity.m_xFixed;
	m_velocity.m_yFixed = p_speed * m_velocity.m_yFixed;
	m_velocity.m_zFixed = p_speed * m_velocity.m_zFixed;
	m_velocity.m_xFixed /= root;
	m_velocity.m_yFixed /= root;
	m_velocity.m_zFixed /= root;
}

// FUNCTION: LEMBALL 0x0042a8f0
void CMove3d::Position(CPt3& p_position, int p_time)
{
	int time = p_time - m_startTime;
	int z = (m_velocity.m_zFixed * time >> 12) + m_start.m_z;
	int y = (m_velocity.m_yFixed * time >> 12) + m_start.m_y;
	int x = (m_velocity.m_xFixed * time >> 12) + m_start.m_x;
	p_position.m_x = x;
	p_position.m_y = y;
	p_position.m_z = z;
}
