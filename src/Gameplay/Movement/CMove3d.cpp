#include "CMove3d.h"

#include "Application/GameMain.h"
#include "Engine/Math/CVSMath.h"
#include "Engine/Math/FixedPoint.h"
#include "Gameplay/Geometry/CPt3.h"

#define MOVE3D_MAX_NORMALIZED_COMPONENT 100

// FUNCTION: LEMBALL 0x0042a7d0
void CMove3d::Set(CPt3& p_start, CPt3& p_end, int p_startTime, int p_speed)
{
	m_start = p_start;
	m_startTime = p_startTime;

	int distance;
	int dx = p_end.m_x - p_start.m_x;
	int dy = p_end.m_y - p_start.m_y;
	int dz = p_end.m_z - p_start.m_z;
	while (dx > MOVE3D_MAX_NORMALIZED_COMPONENT || dx < -MOVE3D_MAX_NORMALIZED_COMPONENT ||
		   dy > MOVE3D_MAX_NORMALIZED_COMPONENT || dy < -MOVE3D_MAX_NORMALIZED_COMPONENT ||
		   dz > MOVE3D_MAX_NORMALIZED_COMPONENT || dz < -MOVE3D_MAX_NORMALIZED_COMPONENT) {
		dx /= 4;
		dy /= 4;
		dz /= 4;
	}

	distance = dz * dz;
	distance += dy * dy;
	distance += dx * dx;
	if (distance == 0) {
		m_velocity.m_xFixed = 0;
		m_velocity.m_yFixed = 0;
		m_velocity.m_zFixed = 0;
		return;
	}

	int root = ((CVSMath*) g_pRandomSeed)->SqRoot(distance);
	m_velocity.m_xFixed = dx * FIXED_POINT_ONE;
	m_velocity.m_yFixed = dy * FIXED_POINT_ONE;
	m_velocity.m_zFixed = dz * FIXED_POINT_ONE;
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
	int z = (m_velocity.m_zFixed * time >> FIXED_POINT_FRACTION_BITS) + m_start.m_z;
	int y = (m_velocity.m_yFixed * time >> FIXED_POINT_FRACTION_BITS) + m_start.m_y;
	int x = (m_velocity.m_xFixed * time >> FIXED_POINT_FRACTION_BITS) + m_start.m_x;
	p_position.m_x = x;
	p_position.m_y = y;
	p_position.m_z = z;
}
