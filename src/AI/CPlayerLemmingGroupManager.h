#ifndef LEMBALL_CPLAYERLEMMINGGROUPMANAGER_H
#define LEMBALL_CPLAYERLEMMINGGROUPMANAGER_H

#include "AI/CPlayerLemmingGroup.h"
#include "Platform/Windows/Mixed/Engine/CORE/LINKSCF.H"

class CPlayerLemming;

class CPlayerLemmingGroupManager : public CGenericGroupManager {
public:
	char m_abReservedB0[0x78];
	int m_nDeadCount128;
	CPlayerLemming* m_apDeadLemmings12C[4];
	CPlayerLemming* m_apNetworkLemmings13C[4];
	int m_fNetworkInitialised14C;

	CPlayerLemming* GetDead(void);
	int GetLeaderPos(AICOORD& position);
	void ProcessDead(void);
	int IsLemmingPlayerControlled(CPlayerLemming* pLemming);
	CPlayerLemmingGroup* GetPlayerControlledGroup(void);
	void AddNewWaypointToCurrentGroup(int nX, int nY);
	void RemoveWaypointsFromCurrentGroup(void);
	void InitialiseNetwork(void);
	int HasSFXChanged(void);
	void AddPlayerLemmingToGroup(CPlayerLemming* pLemming, CPlayerLemmingGroup* pGroup);
};

typedef char CPlayerLemmingGroupManagerSizeCheck[sizeof(CPlayerLemmingGroupManager) == 0x150 ? 1 : -1];

#endif
