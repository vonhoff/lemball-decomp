#include "BulletManager.h"

// 68K 0x10604eb2 __ct__14CBulletManagerFv
// STUB: LEMBALL 0x00417d80
BulletManager::BulletManager()
{
}

// 68K 0x10604fc2 Restart__14CBulletManagerFv
// STUB: LEMBALL 0x00417e80
void BulletManager::Restart()
{
}

// 68K 0x106050a0 NextFreeBullet__14CBulletManagerFv
// STUB: LEMBALL 0x00417ee0
Bullet* BulletManager::NextFreeBullet()
{
	return 0;
}

// 68K 0x10605114 GetFirstBullet__14CBulletManagerFv
// FUNCTION: LEMBALL 0x00417f40
Bullet* BulletManager::GetFirstBullet()
{
	m_iterator = 0;
	return m_activeBullets[0];
}

// 68K 0x1060514e GetNextBullet__14CBulletManagerFv
// FUNCTION: LEMBALL 0x00417f50
Bullet* BulletManager::GetNextBullet()
{
	int iterator = m_iterator + 1;
	m_iterator = iterator;
	if (m_activeCount <= iterator) {
		return 0;
	}
	return m_activeBullets[iterator];
}

// 68K 0x1060519a RequestRemoteBullet__14CBulletManagerFP7CBullet
// STUB: LEMBALL 0x00417f80
void BulletManager::RequestRemoteBullet(Bullet* p_bullet)
{
}

// 68K 0x106051e8 RequestBullet__14CBulletManagerFUs11eBulletType6eOwneri7AICOORD7AICOORD
// STUB: LEMBALL 0x00417fa0
Bullet* BulletManager::RequestBullet(unsigned short p_id,
									 eBulletType p_bulletType,
									 eOwner p_owner,
									 int p_sourceObjectId,
									 AiCoord p_start,
									 AiCoord p_target)
{
	return 0;
}

// 68K 0x106052d4 Process__14CBulletManagerFv
// STUB: LEMBALL 0x00418040
void BulletManager::Process()
{
}

// 68K 0x10605340 RemoveBullet__14CBulletManagerFP7CBullet
// STUB: LEMBALL 0x00418080
void BulletManager::RemoveBullet(Bullet* p_bullet)
{
}

// 68K 0x106053c2 GetViewData__14CBulletManagerFP9CViewData
// STUB: LEMBALL 0x004180e0
int BulletManager::GetViewData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x10605444 CheckGroupIntersection__14CBulletManagerFP7CVSRectP7AICOORD
// STUB: LEMBALL 0x00418120
bool BulletManager::CheckGroupIntersection(VsRect* p_rect, AiCoord* p_coordinate)
{
	return 0;
}

// 68K 0x1060502c __dt__14CBulletManagerFv
BulletManager::~BulletManager()
{
}
