#ifndef LEMBALL_AI_GROUPS_CSHEEPGROUP_H
#define LEMBALL_AI_GROUPS_CSHEEPGROUP_H

#include "../Base/AiCoord.h"
#include "CGenericGroup.h"

class CAI;
class CFormationManager;
class CObjectManager;
// SIZE 0x16c
// VTABLE: LEMBALL 0x00494dd8
class CSheepGroup : public CGenericGroup {
public:
	CSheepGroup(CAI* p_ai, CObjectManager* p_objectManager, CFormationManager* p_formationManager);
	bool CheckAgainstCatapults();
	void CheckAgainstLemmings();
	void RunAway(AiCoord p_threatPosition);
	virtual bool Process(); // vtable+0x14

private:
	int m_runAwayActive; // 0x168
};

// SYNTHETIC: LEMBALL 0x0041f960
// CSheepGroup::`scalar deleting destructor'

#endif
