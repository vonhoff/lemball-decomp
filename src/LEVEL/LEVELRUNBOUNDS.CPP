#include "ENGINE/CORE/VSINIT.H"

struct LevelBounds3View {
	int m_nMinX;
	int m_nMinY;
	int m_nMinZ;
	int m_nMaxX;
	int m_nMaxY;
	int m_nMaxZ;
};

struct LevelManagedEntityBoundsView {
	char m_abReserved00[0x14];
	LevelBounds3View m_bounds14;
	char m_abReserved2C[0x70];
	int m_nPositionX9C;
	int m_nPositionYA0;
	int m_nPositionZA4;
};

// GLOBAL: LEMBALL 0x0049cf58
void* g_pManagedEntityReachabilityHelper = NULL;

// Split from LEVELRUN.CPP to preserve compiler state in the original translation unit.

// FUNCTION: LEMBALL 0x004167c0
void LEMBALL_FASTCALL UpdateManagedEntityBoundsFromPosition(void* pObject)
{
	LevelManagedEntityBoundsView* pEntity;
	LevelBounds3View bounds;
	int nX;
	int nY;
	int nZ;

	pEntity = (LevelManagedEntityBoundsView*) pObject;
	nX = pEntity->m_nPositionX9C >> 12;
	nY = pEntity->m_nPositionYA0 >> 12;
	nX -= 8;
	nY -= 8;
	nZ = pEntity->m_nPositionZA4 >> 12;
	bounds.m_nMinX = nX;
	bounds.m_nMinY = nY;
	bounds.m_nMinZ = nZ;
	bounds.m_nMaxX = nX + 15;
	bounds.m_nMaxY = nY + 15;
	bounds.m_nMaxZ = nZ + 15;
	pEntity->m_bounds14 = bounds;
}
