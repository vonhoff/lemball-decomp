#ifndef LEMBALL_AI_MANAGERS_CBULLETMANAGER_H
#define LEMBALL_AI_MANAGERS_CBULLETMANAGER_H

#include "../Base/AiCoord.h" // complete type
#include "../Objects/CBullet.h"
#include "CBaseObjectManager.h" // complete type

class CVsRect;
// SIZE 0xe0
// VTABLE: LEMBALL 0x00494008
class CBulletManager : public CBaseObjectManager {
public:
	CBullet* GetFirstBullet();
	CBullet* GetNextBullet();
	int GetBulletCount();
	CBullet* NextFreeBullet();
	bool RequestBullet(unsigned short p_id,
					   eBulletType p_bulletType,
					   eOwner p_owner,
					   int p_sourceObjectId,
					   AiCoord p_start,
					   AiCoord p_target);
	CBulletManager();
	bool CheckGroupIntersection(CVsRect* p_rect, AiCoord* p_coordinate);
	int GetViewData(CViewData* p_viewData);
	virtual ~CBulletManager(); // vtable+0x14
	void Process();
	void RemoveBullet(CBullet* p_bullet);
	void RequestRemoteBullet(CBullet* p_bullet);
	void Restart();
	CBullet* GetCurrentBullet();

private:
	CBullet* m_bullets;           // 0x30
	CBullet* m_activeBullets[40]; // 0x34
	int m_activeCount;            // 0xd4
	int m_iterator;               // 0xd8
	int m_poolStart;              // 0xdc
};

// SYNTHETIC: LEMBALL 0x00418300
// CBulletManager::`scalar deleting destructor'

#endif
