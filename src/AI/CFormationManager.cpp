#include "AI/CFormationManager.h"

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
