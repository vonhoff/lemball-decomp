#ifndef LEMBALL_AI_GROUPS_PLAYERLEMMINGGROUP_H
#define LEMBALL_AI_GROUPS_PLAYERLEMMINGGROUP_H

#include "../../Common.h"
#include "GenericGroup.h" // complete type

// SIZE 0x174
// VTABLE: LEMBALL 0x00493a60
class PlayerLemmingGroup : public GenericGroup {
public:
	PlayerLemming* GetFirstDeadLemming();
	PlayerLemmingGroup(Ai* p_arg0, ObjectManager* p_arg1, FormationManager* p_arg2);
	bool AddLemmingToGroup(PlayerLemming* p_lemming);
	bool CheckPlayerControlled();
	bool HasSfxChanged();
	bool RemoveLemmingFromGroup(PlayerLemming* p_lemming);
	virtual bool Process();                         // vtable+0x14
	virtual void ClearExistingWaypoints();          // vtable+0x150
	virtual void Delete();                          // vtable+0xc8
	virtual void GetViewData(ViewData* p_viewData); // vtable+0x158
	virtual void Restart();                         // vtable+0x104
	virtual ~PlayerLemmingGroup();                  // vtable+0x00
	void AddUseObject(GameObject* p_object, int p_objectId);
	void AddUseObject(int p_objectId);
	void SetPlayerControlled(unsigned char p_playerControlled, PlayerLemming* p_leader);

private:
	int m_playerControlled;  // 0x168
	int m_currentUseElement; // 0x16c
	GameObject* m_useObject; // 0x170
};

#endif
