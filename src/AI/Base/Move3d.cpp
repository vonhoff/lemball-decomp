#include "Move3d.h"

#include "../../Visos/Foundation/VsMath.h"

// 68K 0x10616f8c Set__7CMove3dFR4CPt3R4CPt3ii
// FUNCTION: LEMBALL 0x0042a7d0
void Move3d::Set(Pt3& p_start, Pt3& p_end, int p_startTime, int p_speed)
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
		m_velocity.m_x = 0;
		m_velocity.m_y = 0;
		m_velocity.m_z = 0;
		return;
	}

	int root = ((VsMath*) g_pSentinel)->SqRoot(distance);
	m_velocity.m_x = dx * 0x1000;
	m_velocity.m_y = dy * 0x1000;
	m_velocity.m_z = dz * 0x1000;
	m_velocity.m_x = p_speed * m_velocity.m_x;
	m_velocity.m_y = p_speed * m_velocity.m_y;
	m_velocity.m_z = p_speed * m_velocity.m_z;
	m_velocity.m_x /= root;
	m_velocity.m_y /= root;
	m_velocity.m_z /= root;
}

// 68K 0x1061710a Position__7CMove3dFR4CPt3i
// FUNCTION: LEMBALL 0x0042a8f0
void Move3d::Position(Pt3& p_position, int p_time)
{
	int time = p_time - m_startTime;
	int z = (m_velocity.m_z * time >> 12) + m_start.m_z;
	int y = (m_velocity.m_y * time >> 12) + m_start.m_y;
	int x = (m_velocity.m_x * time >> 12) + m_start.m_x;
	p_position.m_x = x;
	p_position.m_y = y;
	p_position.m_z = z;
}
