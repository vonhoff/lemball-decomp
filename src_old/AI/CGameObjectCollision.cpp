#include "AI/CGameObject.h"
#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

// GLOBAL: LEMBALL 0x0049cf58
void* g_pManagedEntityReachabilityHelper = NULL;

// FUNCTION: LEMBALL 0x004167c0
void CGameObject::UpdateCollision(void)
{
	CGameObjectBounds bounds;
	int nX;
	int nY;
	int nZ;

	nX = m_WorldPosition9C.x >> 12;
	nY = m_WorldPosition9C.y >> 12;
	nX -= 8;
	nY -= 8;
	nZ = m_WorldPosition9C.z >> 12;
	bounds.m_nMinX = nX;
	bounds.m_nMinY = nY;
	bounds.m_nMinZ = nZ;
	bounds.m_nMaxX = nX + 15;
	bounds.m_nMaxY = nY + 15;
	bounds.m_nMaxZ = nZ + 15;
	m_bounds14 = bounds;
}
