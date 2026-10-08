#ifndef LEMBALL_AI_MANAGERS_CBULLETMANAGER_H
#define LEMBALL_AI_MANAGERS_CBULLETMANAGER_H

#include "CBullet.h"
#include "Gameplay/Objects/CBaseObjectManager.h"

class CVSRect;
class AICOORD;

enum {
	BULLET_ACTIVE_LIST_CAPACITY = 40,
	BULLET_OBJECT_POOL_PARTITION_CAPACITY = BULLET_ACTIVE_LIST_CAPACITY / 2,
	BULLET_ACTIVE_LIST_SEARCH_COUNT = 20,
	BULLET_ACTIVE_LIST_SEARCH_LAST_INDEX = BULLET_ACTIVE_LIST_SEARCH_COUNT - 1
};
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
					   AICOORD p_start,
					   AICOORD p_target);
	CBulletManager();
	bool CheckGroupIntersection(CVSRect* p_rect, AICOORD* p_coordinate);
	int GetViewData(CViewData* p_viewData);
	virtual ~CBulletManager(); // vtable+0x14
	void Process();
	void RemoveBullet(CBullet* p_bullet);
	void RequestRemoteBullet(CBullet* p_bullet);
	void Restart();
	CBullet* GetCurrentBullet();

private:
	CBullet* m_bullets;                                    // 0x30
	CBullet* m_activeBullets[BULLET_ACTIVE_LIST_CAPACITY]; // 0x34
	int m_activeCount;                                     // 0xd4
	int m_iterator;                                        // 0xd8
	int m_poolStart;                                       // 0xdc
};

// SYNTHETIC: LEMBALL 0x00418300
// CBulletManager::`scalar deleting destructor'

#endif
