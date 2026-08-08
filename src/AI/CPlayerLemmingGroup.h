#ifndef LEMBALL_CPLAYERLEMMINGGROUP_H
#define LEMBALL_CPLAYERLEMMINGGROUP_H

#include "AI/CGenericGroup.h"

class CPlayerLemming;
struct LevelManagedEntityTargetView;

class CPlayerLemmingGroup : public CGenericGroup {
public:
	int m_nRuntimeState160;
	int m_nRuntimeState164;
	int m_nRuntimeState168;
	int m_nReserved16C;
	LevelManagedEntityTargetView* m_pActiveTarget170;

	void Restart(void);
	void Delete(void);
	int AddLemmingToGroup(CPlayerLemming* pLemming);
	void SetPlayerControlled(int fPlayerControlled, CPlayerLemming* pLeader);
	int CheckPlayerControlled(void);
	CPlayerLemming* GetFirstDeadLemming(void);
	int HasSFXChanged(void);
	void AddUseObject(int nId);
	void AddUseObject(CGameObject* pObject, int nId);
	int RemoveLemmingFromGroup(CPlayerLemming* pLemming);
	int GetViewData(int pViewData);
};

#endif
