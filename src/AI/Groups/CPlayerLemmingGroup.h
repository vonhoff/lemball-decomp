#ifndef LEMBALL_AI_GROUPS_CPLAYERLEMMINGGROUP_H
#define LEMBALL_AI_GROUPS_CPLAYERLEMMINGGROUP_H

#include "CGenericGroup.h"

class CAI;
class CFormationManager;
class CGameObject;
class CObjectManager;
class CPlayerLemming;
// SIZE 0x174
// VTABLE: LEMBALL 0x00493a60
class CPlayerLemmingGroup : public CGenericGroup {
public:
	CPlayerLemming* GetFirstDeadLemming();
	CPlayerLemmingGroup(CAI* p_ai, CObjectManager* p_objectManager, CFormationManager* p_formationManager);
	bool AddLemmingToGroup(CPlayerLemming* p_lemming);
	bool CheckPlayerControlled();
	bool HasSFXChanged();
	bool RemoveLemmingFromGroup(CPlayerLemming* p_lemming);
	virtual bool Process();                         // vtable+0x14
	virtual void ClearExistingWaypoints();          // vtable+0x150
	virtual void Delete();                          // vtable+0xc8
	virtual int GetViewData(CViewData* p_viewData); // vtable+0x158
	virtual void Restart();                         // vtable+0x104
	virtual ~CPlayerLemmingGroup();                 // vtable+0x00
	void AddUseObject(CGameObject* p_object, int p_objectId);
	void AddUseObject(int p_objectId);
	void SetPlayerControlled(unsigned int p_playerControlled, CPlayerLemming* p_leader);

private:
	int m_playerControlled;   // 0x168
	int m_currentUseElement;  // 0x16c
	CGameObject* m_useObject; // 0x170
};

// SYNTHETIC: LEMBALL 0x00414c80
// CPlayerLemmingGroup::`scalar deleting destructor'

#endif
