#ifndef LEMBALL_AI_MANAGERS_BULLETMANAGER_H
#define LEMBALL_AI_MANAGERS_BULLETMANAGER_H

#include "../../Common.h"
#include "../Base/AiCoord.h"   // complete type
#include "BaseObjectManager.h" // complete type

// SIZE 0xe0
// VTABLE: LEMBALL 0x00494008
class BulletManager : public BaseObjectManager {
public:
	Bullet* GetFirstBullet();
	Bullet* GetNextBullet();
	Bullet* NextFreeBullet();
	bool RequestBullet(unsigned short p_id,
					   eBulletType p_bulletType,
					   eOwner p_owner,
					   int p_sourceObjectId,
					   AiCoord p_start,
					   AiCoord p_target);
	BulletManager();
	bool CheckGroupIntersection(VsRect* p_rect, AiCoord* p_coordinate);
	int GetViewData(ViewData* p_viewData);
	virtual ~BulletManager(); // vtable+0x14
	void Process();
	void RemoveBullet(Bullet* p_bullet);
	void RequestRemoteBullet(Bullet* p_bullet);
	void Restart();

private:
	Bullet* m_bullets;           // 0x30
	Bullet* m_activeBullets[40]; // 0x34
	int m_activeCount;           // 0xd4
	int m_iterator;              // 0xd8
	int m_poolStart;             // 0xdc
};

// SYNTHETIC: LEMBALL 0x00418300
// BulletManager::`scalar deleting destructor'

#endif
