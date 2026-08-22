#ifndef LEMBALL_AI_GROUPS_FORMATIONMANAGER_H
#define LEMBALL_AI_GROUPS_FORMATIONMANAGER_H

#include "../../Common.h"
#include "../../Visos/Foundation/Vector.h" // complete type

// SIZE 0x104
class FormationManager {
public:
	FormationManager();
	Vector* GetAVector(int p_index);
	void Restart();
	void TransformFormation(int p_formationIndex, int p_angle);
	~FormationManager();

private:
	Vector m_sourceVectors[24];     // 0x000
	Vector m_transformedVectors[8]; // 0x0c0
	int m_restartState;             // 0x100
};

// Confirmed class-scoped globals.
extern FormationManager* g_pGenericGroupFormationManager;
#endif
