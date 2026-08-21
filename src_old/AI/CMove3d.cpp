#include "AI/CMove3d.h"

#include "Visos/Generic/CVSMath.h"

extern void* g_pSessionRandomState;

// FUNCTION: LEMBALL 0x0042a7d0
void CMove3d::Set(CPt3& start, CPt3& end, int nStartTime, int nSpeed)
{
	int dx;
	int dy;
	int dz;
	unsigned int nLengthSquared;
	int nLength;

	m_Start = start;
	m_nStartTime = nStartTime;
	dx = end.x - start.x;
	dy = end.y - start.y;
	dz = end.z - start.z;
	while (dx > 100 || dx < -100 || dy > 100 || dy < -100 || dz > 100 || dz < -100) {
		dx /= 4;
		dy /= 4;
		dz /= 4;
	}
	nLengthSquared = dx * dx + dy * dy + dz * dz;
	if (nLengthSquared == 0) {
		m_Velocity.x = 0;
		m_Velocity.y = 0;
		m_Velocity.z = 0;
		return;
	}
	nLength = ((CVSMath*) g_pSessionRandomState)->SqRoot(nLengthSquared);
	m_Velocity.x = nSpeed * (dx << 12) / nLength;
	m_Velocity.y = nSpeed * (dy << 12) / nLength;
	m_Velocity.z = nSpeed * (dz << 12) / nLength;
}

// FUNCTION: LEMBALL 0x0042a8f0
void CMove3d::Position(CPt3& position, int nTime)
{
	int nElapsed = nTime - m_nStartTime;
	int nVelocityZ = m_Velocity.z;
	int nVelocityY = m_Velocity.y;
	int nStartZ = m_Start.z;
	int nStartY = m_Start.y;

	position.x = ((m_Velocity.x * nElapsed) >> 12) + m_Start.x;
	position.y = ((nVelocityY * nElapsed) >> 12) + nStartY;
	position.z = ((nVelocityZ * nElapsed) >> 12) + nStartZ;
}
