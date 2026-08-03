#include "AI/CFormationManager.h"

struct LevelProjectileGeometryPair {
	LevelProjectileGeometryPair* InitializeSentinel(void);
};

extern int g_LEVEL_ProjectileGeometryPairSeeds[3][8][2];

// FUNCTION: LEMBALL 0x0041a140
CFormationManager::CFormationManager(void)
{
	int i;
	int j;
	CFormationVector* pVector;

	for (i = 0; i < 24; ++i) {
		((LevelProjectileGeometryPair*) &m_aFormationVectors00[i])->InitializeSentinel();
	}
	for (i = 0; i < 8; ++i) {
		((LevelProjectileGeometryPair*) &m_aTransformedVectorsC0[i])->InitializeSentinel();
	}

	pVector = m_aFormationVectors00;
	for (i = 0; i < 3; ++i) {
		for (j = 0; j < 8; ++j) {
			pVector->x = g_LEVEL_ProjectileGeometryPairSeeds[i][j][0] << 12;
			pVector->y = g_LEVEL_ProjectileGeometryPairSeeds[i][j][1] << 12;
			++pVector;
		}
	}
}

// FUNCTION: LEMBALL 0x0041a1b0
void CFormationManager::Restart(void)
{
	m_nVectorCursor100 = 0;
}

// FUNCTION: LEMBALL 0x0041a1c0
CFormationManager::~CFormationManager(void)
{
}

// FUNCTION: LEMBALL 0x0041a320
CFormationVector* CFormationManager::GetAVector(int nIndex)
{
	if (nIndex >= 8) {
		nIndex -= 8;
	}
	return &m_aTransformedVectorsC0[nIndex];
}
