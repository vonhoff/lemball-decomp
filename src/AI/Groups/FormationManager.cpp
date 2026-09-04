#include "FormationManager.h"

// 68K 0x10608de2 __ct__17CFormationManagerFv
// STUB: LEMBALL 0x0041a140
FormationManager::FormationManager()
{
}

// 68K 0x10608e92 Restart__17CFormationManagerFv
// FUNCTION: LEMBALL 0x0041a1b0
void FormationManager::Restart()
{
	m_restartState = 0;
}

// 68K 0x10608ec4 __dt__17CFormationManagerFv
// FUNCTION: LEMBALL 0x0041a1c0
FormationManager::~FormationManager()
{
}

// 68K 0x10608f08 TransformFormation__17CFormationManagerFii
// STUB: LEMBALL 0x0041a1d0
void FormationManager::TransformFormation(int p_formationIndex, int p_angle)
{
}

// 68K 0x10608fa4 GetAVector__17CFormationManagerFi
// FUNCTION: LEMBALL 0x0041a320
Vector* FormationManager::GetAVector(int p_index)
{
	if (p_index >= 8) {
		p_index -= 8;
	}
	return &m_transformedVectors[p_index];
}

// GLOBAL: LEMBALL 0x004a7834
FormationManager* g_pGenericGroupFormationManager;
