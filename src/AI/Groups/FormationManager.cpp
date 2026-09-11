#include "FormationManager.h"

#include "../../Visos/Foundation/VsTrig.h"

// 68K 0x10608de2 __ct__17CFormationManagerFv
// FUNCTION: LEMBALL 0x0041a140
FormationManager::FormationManager()
{
	Vector* formation;
	Vector* source;
	int formationCount;
	int vectorCount;
	int y;
	int x;

	source = m_sourceVectors;
	formation = (Vector*) g_aFormationTemplates;
	formationCount = 3;
	do {
		vectorCount = 8;
		do {
			y = formation->m_yFixed;
			x = formation->m_xFixed;
			x <<= 12;
			source++;
			y <<= 12;
			formation++;
			vectorCount--;
			source[-1].m_xFixed = x;
			source[-1].m_yFixed = y;
		} while (vectorCount != 0);
		formationCount--;
	} while (formationCount != 0);
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
// FUNCTION: LEMBALL 0x0041a1d0
void FormationManager::TransformFormation(int p_formationIndex, int p_angle)
{
	Vector* source;
	Vector* transformed;
	int remaining;

	source = m_sourceVectors + p_formationIndex * 8;
	transformed = m_transformedVectors;
	remaining = 8;
	do {
		Fixed sin = g_pVSTrig->Sin(p_angle);
		Fixed cos = g_pVSTrig->Cos(p_angle);
		Vector rotated = g_pVSTrig->Rotate(*source, sin, cos);
		*transformed = rotated;
		source++;
		transformed++;
		remaining--;
	} while (remaining != 0);
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
