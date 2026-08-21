#ifndef LEMBALL_CPLAYERLEMMINGGROUPMANAGER_H
#define LEMBALL_CPLAYERLEMMINGGROUPMANAGER_H

#include "AI/CPlayerLemmingGroup.h"
#include "Platform/Windows/Mixed/Engine/CORE/LINKSCF.H"

class CPlayerLemming;

class CPlayerLemmingGroupManager : public CGenericGroupManager {
public:
	char m_abReservedB0[0x70];
	int m_nCurrentPlayerGroup120;
	int m_nReserved124;
	int m_nDeadCount128;
	CPlayerLemming* m_apDeadLemmings12C[4];
	CPlayerLemming* m_apNetworkLemmings13C[4];
	int m_fNetworkInitialised14C;

	CPlayerLemming* GetDead(void);
	int GetLeaderPos(AICOORD& position);
	void ProcessDead(void);
	void Process(void);
	void CreateNewGroup(unsigned short cLemmings, unsigned short* pLemmingIds);
	int IsLemmingPlayerControlled(CPlayerLemming* pLemming);
	int MakeNextGroupPlayerControlled(void);
	int MakePreviousGroupPlayerControlled(void);
	int MakeParticularGroupPlayerControlled(CPlayerLemmingGroup* pGroup);
	int MakeNoGroupsPlayerControlled(void);
	CPlayerLemmingGroup* GetPlayerControlledGroup(void);
	void AddNewWaypointToCurrentGroup(int nX, int nY);
	void RemoveWaypointsFromCurrentGroup(void);
	void InitialiseNetwork(void);
	void LoadAdditionalPlayerStartPositions(unsigned short* pData, int nUnused, int fReuseNetworkObjects);
	int HasSFXChanged(void);
	void AddPlayerLemmingToGroup(CPlayerLemming* pLemming, CPlayerLemmingGroup* pGroup);
	int GetViewData(int pViewData);
	void UseObject(unsigned int nObject);
	void PlayerGroupRequestFire(int nX, int nY);
	void ReformAlteredGroups(CPlayerLemmingGroup* pGroup);
	~CPlayerLemmingGroupManager(void);
};

typedef char CPlayerLemmingGroupManagerSizeCheck[sizeof(CPlayerLemmingGroupManager) == 0x150 ? 1 : -1];

#endif
