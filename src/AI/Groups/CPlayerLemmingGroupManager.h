#ifndef LEMBALL_AI_GROUPS_CPLAYERLEMMINGGROUPMANAGER_H
#define LEMBALL_AI_GROUPS_CPLAYERLEMMINGGROUPMANAGER_H

#include "../Managers/CBaseObjectManager.h" // complete type
#include "CGenericGroupManager.h"           // complete type

class CAi;
class AiCoord;
class CFormationManager;
class CObjectManager;
class CPlayerLemming;
class CPlayerLemmingGroup;
class CViewData;
// SIZE 0x150
// VTABLE: LEMBALL 0x00494068 CGenericGroupManager
// VTABLE: LEMBALL 0x00494038 CBaseObjectManager
class CPlayerLemmingGroupManager : public CGenericGroupManager, public CBaseObjectManager {
public:
	CPlayerLemming* GetDead();
	CPlayerLemmingGroup* GetPlayerControlledGroup();
	CPlayerLemmingGroupManager(CAi* p_arg0, CObjectManager* p_arg1, CFormationManager* p_arg2);
	bool GetLeaderPos(AiCoord& p_position);
	bool HasSfxChanged();
	bool IsLemmingPlayerControlled(CPlayerLemming* p_lemming);
	bool MakeNextGroupPlayerControlled();
	bool MakeNoGroupsPlayerControlled();
	bool MakeParticularGroupPlayerControlled(CPlayerLemmingGroup* p_group);
	bool MakePreviousGroupPlayerControlled();
	virtual int GetViewData(CViewData* p_viewData);                                   // vtable+0x4c
	virtual void CreateNewGroup(unsigned short p_count, unsigned short* p_objectIds); // vtable+0x34
	void DeleteGroup(CPlayerLemmingGroup* p_group);
	void Process();
	void Restart();
	virtual ~CPlayerLemmingGroupManager(); // vtable+0x14
	void AddNewWaypointToCurrentGroup(int p_x, int p_y);
	void AddPlayerLemmingToGroup(CPlayerLemming* p_lemming, CPlayerLemmingGroup* p_group);
	void InitialiseNetwork();
	void LoadAdditionalPlayerStartPositions(unsigned char* p_data, unsigned long p_dataSize, unsigned int p_skip);
	void LoadLevel(unsigned char* p_data, unsigned long p_dataSize, unsigned int p_skip);
	void PlayerGroupRequestFire(int p_x, int p_y);
	void ProcessDead();
	void ReformAlteredGroups(CPlayerLemmingGroup* p_excludedGroup);
	void RemoveWaypointsFromCurrentGroup();
	void UseObject(int p_objectId);

private:
	friend class CAi;
	int m_startX[4];                      // 0x0e0
	int m_startY[4];                      // 0x0f0
	int m_startZ[4];                      // 0x100
	int m_lemmingCounts[4];               // 0x110
	int m_controlledGroupIndex;           // 0x120
	int m_startPositionCount;             // 0x124
	int m_deadCount;                      // 0x128
	CPlayerLemming* m_dead[4];            // 0x12c
	CPlayerLemming* m_networkLemmings[4]; // 0x13c
	int m_networkInitialized;             // 0x14c

public:
	void GetPlayerStartPosition(AiCoord& p_position, int p_index);
	void ConfigurePlayerLemmingCounts(int p_playerCount, int p_count0, int p_count1, int p_count2, int p_count3);
	int GetLemmingCountForPlayer(int p_playerIndex);
};

// SYNTHETIC: LEMBALL 0x00419930
// CPlayerLemmingGroupManager::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00419970
// ?GetViewData@CPlayerLemmingGroupManager@@WLA@AEHPAVCViewData@@@Z

#endif
