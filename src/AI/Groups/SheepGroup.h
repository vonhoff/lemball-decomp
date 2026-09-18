#ifndef LEMBALL_AI_GROUPS_SHEEPGROUP_H
#define LEMBALL_AI_GROUPS_SHEEPGROUP_H

#include "../Base/AiCoord.h" // complete type
#include "GenericGroup.h"    // complete type

class Ai;
class FormationManager;
class ObjectManager;
// SIZE 0x16c
// VTABLE: LEMBALL 0x00494dd8
class SheepGroup : public GenericGroup {
public:
	SheepGroup(Ai* p_ai, ObjectManager* p_objectManager, FormationManager* p_formationManager);
	bool CheckAgainstCatapults();
	void CheckAgainstLemmings();
	void RunAway(AiCoord p_threatPosition);
	virtual bool Process(); // vtable+0x14

private:
	int m_runAwayActive; // 0x168
};

// SYNTHETIC: LEMBALL 0x0041f960
// SheepGroup::`scalar deleting destructor'

#endif
