#ifndef LEMBALL_AI_GROUPS_CFORMATIONMANAGER_H
#define LEMBALL_AI_GROUPS_CFORMATIONMANAGER_H

#include "Engine/Math/CVector.h"

enum FormationVectorComponent {
	kFormationVectorX,
	kFormationVectorY,
};

typedef int FormationVectorTemplate[2];

// SIZE 0x104
class CFormationManager {
public:
	CFormationManager();
	CVector* GetAVector(int p_index);
	CVector* GetFirstVector();
	CVector* GetNextVector();
	void Restart();
	void TransformFormation(int p_formationIndex, int p_angle);
	~CFormationManager();

private:
	CVector m_sourceVectors[24];     // 0x000
	CVector m_transformedVectors[8]; // 0x0c0
	int m_restartState;              // 0x100
};

extern CFormationManager* g_pGenericGroupFormationManager;
extern FormationVectorTemplate g_aFormationTemplates[24];
#endif
