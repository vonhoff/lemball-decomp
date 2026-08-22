#ifndef LEMBALL_AI_GROUPS_SHEEPGROUP_H
#define LEMBALL_AI_GROUPS_SHEEPGROUP_H

#include "../../Common.h"
#include "../Base/AiCoord.h" // complete type
#include "GenericGroup.h"    // complete type

// SIZE 0x16c
// VTABLE: LEMBALL 0x00494dd8
class SheepGroup : public GenericGroup {
public:
	SheepGroup(Ai* p_arg0, ObjectManager* p_arg1, FormationManager* p_arg2);
	bool CheckAgainstCatapults();
	bool CheckAgainstLemmings();
	bool RunAway(AiCoord p_threatPosition);
	virtual bool Process(); // vtable+0x14
	virtual ~SheepGroup();  // vtable+0x00

private:
	int m_runAwayActive; // 0x168
};

#endif
