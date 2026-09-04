#include "BulletManager.h"

#include "../../Visos/Network/Connect.h"
#include "../Base/GameObject.h"
#include "../Objects/Bullet.h"
#include "../Objects/ViewData.h"

// 68K 0x10604eb2 __ct__14CBulletManagerFv
// FUNCTION: LEMBALL 0x00417d80
BulletManager::BulletManager() : BaseObjectManager(0x21, 0x16)
{
	m_bullets = new Bullet[40];
	for (int i = 0; i < 40; i++) {
		m_activeBullets[i] = 0;
		m_bullets[i].SetId(GameObject::NextLoadingId());
		m_bullets[i].m_manager = this;
	}
	if (g_pActiveConnection != 0 && g_pActiveConnection->m_isHost != 0) {
		m_poolStart = 0x14;
		return;
	}
	m_poolStart = 0;
}

// 68K 0x10604fc2 Restart__14CBulletManagerFv
// FUNCTION: LEMBALL 0x00417e80
void BulletManager::Restart()
{
	m_iterator = 0;
	m_activeCount = 0;
	for (int i = 0; i < 40; i++) {
		m_activeBullets[i] = 0;
		m_bullets[i].Restart();
	}
}

// 68K 0x106050a0 NextFreeBullet__14CBulletManagerFv
// FUNCTION: LEMBALL 0x00417ee0
Bullet* BulletManager::NextFreeBullet()
{
	for (int i = 0; i < 0x14; i++) {
		if (m_bullets[m_poolStart + i].m_active == 0) {
			return m_bullets + m_poolStart + i;
		}
	}
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
// FUNCTION: LEMBALL 0x00417f80
void BulletManager::RequestRemoteBullet(Bullet* p_bullet)
{
	m_activeBullets[m_activeCount] = p_bullet;
	m_activeCount = m_activeCount + 1;
}

// 68K 0x106051e8 RequestBullet__14CBulletManagerFUs11eBulletType6eOwneri7AICOORD7AICOORD
// FUNCTION: LEMBALL 0x00417fa0
bool BulletManager::RequestBullet(unsigned short p_id,
								  eBulletType p_bulletType,
								  eOwner p_owner,
								  int p_sourceObjectId,
								  AiCoord p_start,
								  AiCoord p_target)
{
	if (m_activeCount < 0x28) {
		m_activeBullets[m_activeCount] = NextFreeBullet();
		if (m_activeBullets[m_activeCount] != 0) {
			m_activeBullets[m_activeCount]->Set(p_id, p_bulletType, p_owner, p_sourceObjectId, p_start, p_target);
			m_activeBullets[m_activeCount]->FireBullet();
			m_activeCount = m_activeCount + 1;
		}
		return 1;
	}
	return 0;
}

// 68K 0x106052d4 Process__14CBulletManagerFv
// FUNCTION: LEMBALL 0x00418040
void BulletManager::Process()
{
	Bullet* bullet = GetFirstBullet();
	while (bullet != 0) {
		if (bullet->Process() == 0) {
			RemoveBullet(bullet);
		}
		bullet = GetNextBullet();
	}
}

// 68K 0x10605340 RemoveBullet__14CBulletManagerFP7CBullet
// FUNCTION: LEMBALL 0x00418080
void BulletManager::RemoveBullet(Bullet* p_bullet)
{
	for (int i = 0; i < 0x14; i++) {
		if (p_bullet == m_activeBullets[i]) {
			Bullet** slot = &m_activeBullets[i];
			m_activeBullets[i]->Free();
			if (i < 0x13) {
				int count = 0x13 - i;
				i += count;
				do {
					*slot = slot[1];
					slot++;
					count--;
				} while (count != 0);
			}
			m_activeBullets[i] = 0;
			m_activeCount--;
			return;
		}
	}
}

// 68K 0x106053c2 GetViewData__14CBulletManagerFP9CViewData
// FUNCTION: LEMBALL 0x004180e0
int BulletManager::GetViewData(ViewData* p_viewData)
{
	Bullet* bullet = GetFirstBullet();
	int count = 0;
	while (bullet != 0) {
		bullet->GetViewData(*p_viewData);
		p_viewData++;
		count++;
		bullet = GetNextBullet();
	}
	return count;
}

// 68K 0x10605444 CheckGroupIntersection__14CBulletManagerFP7CVSRectP7AICOORD
// FUNCTION: LEMBALL 0x00418120
bool BulletManager::CheckGroupIntersection(VsRect* p_rect, AiCoord* p_coordinate)
{
	Bullet* bullet = GetFirstBullet();
	while (bullet != 0) {
		int x = bullet->m_position.m_xFixed >> 0xc;
		int y = bullet->m_position.m_yFixed >> 0xc;
		if (x - 8 < p_rect->m_width + p_rect->m_x && p_rect->m_x < x + 8 && y - 8 < p_rect->m_height + p_rect->m_y &&
			p_rect->m_y < y + 8) {
			p_coordinate->m_xFixed = bullet->m_position.m_xFixed;
			p_coordinate->m_yFixed = bullet->m_position.m_yFixed;
			p_coordinate->m_zFixed = bullet->m_position.m_zFixed;
			return 1;
		}
		bullet = GetNextBullet();
	}
	return 0;
}

// 68K 0x1060502c __dt__14CBulletManagerFv
BulletManager::~BulletManager()
{
}
