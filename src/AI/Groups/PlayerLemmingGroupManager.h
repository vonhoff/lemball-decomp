#ifndef LEMBALL_SCAFFOLD_AI_GROUPS_PLAYERLEMMINGGROUPMANAGER_H
#define LEMBALL_SCAFFOLD_AI_GROUPS_PLAYERLEMMINGGROUPMANAGER_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type
#include "GenericGroupManager.h"                  // complete type

// SIZE 0x150
// VTABLE: LEMBALL 0x00494068 primary GenericGroupManager base
// VTABLE: LEMBALL 0x00494038 secondary NetworkMessage base at +0xb0
class PlayerLemmingGroupManager : public GenericGroupManager, public NetworkMessage {
public:
	PlayerLemming* GetDead();
	PlayerLemmingGroup* GetPlayerControlledGroup();
	PlayerLemmingGroupManager(Ai* p_arg0, ObjectManager* p_arg1, FormationManager* p_arg2);
	bool GetLeaderPos(AiCoord& p_position);
	bool HasSfxChanged();
	bool IsLemmingPlayerControlled(PlayerLemming* p_lemming);
	bool MakeNextGroupPlayerControlled();
	bool MakeNoGroupsPlayerControlled();
	bool MakeParticularGroupPlayerControlled(PlayerLemmingGroup* p_group);
	bool MakePreviousGroupPlayerControlled();
	virtual int GetViewData(ViewData* p_viewData);                                    // vtable+0x4c
	virtual void CreateNewGroup(unsigned short p_count, unsigned short* p_objectIds); // vtable+0x34
	virtual void DeleteGroup(PlayerLemmingGroup* p_group);                            // vtable+0x14
	virtual void Process();                                                           // vtable+0x1c
	virtual void Restart();                                                           // vtable+0x18
	virtual ~PlayerLemmingGroupManager();                                             // vtable+0x14
	void AddNewWaypointToCurrentGroup(int p_x, int p_y);
	void AddPlayerLemmingToGroup(PlayerLemming* p_lemming, PlayerLemmingGroup* p_group);
	void InitialiseNetwork();
	void LoadAdditionalPlayerStartPositions(unsigned char* p_data, unsigned long p_dataSize, unsigned char p_skip);
	void LoadLevel(unsigned char* p_data, unsigned long p_dataSize, unsigned char p_skip);
	void PlayerGroupRequestFire(int p_x, int p_y);
	void ProcessDead();
	void ReformAlteredGroups(PlayerLemmingGroup* p_excludedGroup);
	void RemoveWaypointsFromCurrentGroup();
	void UseObject(int p_objectId);

private:
	int m_messageType;                   // 0x0dc
	int m_startX[4];                     // 0x0e0
	int m_startY[4];                     // 0x0f0
	int m_startZ[4];                     // 0x100
	int m_lemmingCounts[4];              // 0x110
	int m_controlledGroupIndex;          // 0x120
	int m_startPositionCount;            // 0x124
	int m_deadCount;                     // 0x128
	PlayerLemming* m_dead[4];            // 0x12c
	PlayerLemming* m_networkLemmings[4]; // 0x13c
	int m_networkInitialized;            // 0x14c
};

#endif
