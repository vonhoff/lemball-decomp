#ifndef LEMBALL_CPLAYERLEMMINGGROUPMANAGER_H
#define LEMBALL_CPLAYERLEMMINGGROUPMANAGER_H

#include "AI/CPlayerLemmingGroup.h"
#include "Platform/Windows/Mixed/Engine/CORE/LINKSCF.H"

class CPlayerLemming;

class CPlayerLemmingGroupManager : public CGenericGroupManager {
public:
	char m_abReservedB0[0x78];
	int m_nDeadCount128;
	CPlayerLemming* m_apDeadLemmings12C[8];
	int m_nSelectedGroup14C;

	CPlayerLemming* GetDead(void);
	int GetLeaderPos(AICOORD& position);
	int IsLemmingPlayerControlled(CPlayerLemming* pLemming);
	CPlayerLemmingGroup* GetPlayerControlledGroup(void);
	int HasSFXChanged(void);
};

typedef char CPlayerLemmingGroupManagerSizeCheck[sizeof(CPlayerLemmingGroupManager) == 0x150 ? 1 : -1];

#endif
