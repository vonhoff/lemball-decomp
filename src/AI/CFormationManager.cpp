#include "AI/CFormationManager.h"

struct LevelProjectileGeometryPair {
	LevelProjectileGeometryPair* InitializeSentinel(void);
};

extern int g_LEVEL_ProjectileGeometryPairSeeds[3][8][2];
extern void* g_pSignedTrigTable;
extern void __stdcall RotateFormationVector(CFormationVector* pOutput,
											int nX,
											int nY,
											const int* pSin,
											const int* pCos);

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

// FUNCTION: LEMBALL 0x0041a1d0
void CFormationManager::TransformFormation(int nFormation, int nAngle)
{
	CFormationVector* pSource = &m_aFormationVectors00[nFormation * 8];
	CFormationVector* pOutput = m_aTransformedVectorsC0;
	int* pTrigTable = (int*) g_pSignedTrigTable;
	int nSin;
	int nCos;
	int i;

	for (i = 0; i < 8; ++i) {
		if (nAngle < 0) {
			nSin = -pTrigTable[(-nAngle) & 0x1ff];
		}
		else {
			nSin = pTrigTable[nAngle & 0x1ff];
		}
		if (nAngle + 0x80 < 0) {
			nCos = -pTrigTable[(-nAngle - 0x80) & 0x1ff];
		}
		else {
			nCos = pTrigTable[(nAngle + 0x80) & 0x1ff];
		}
		RotateFormationVector(pOutput, pSource->x, pSource->y, &nSin, &nCos);
		++pSource;
		++pOutput;
	}
}

// FUNCTION: LEMBALL 0x0041a320
CFormationVector* CFormationManager::GetAVector(int nIndex)
{
	if (nIndex >= 8) {
		nIndex -= 8;
	}
	return &m_aTransformedVectorsC0[nIndex];
}
