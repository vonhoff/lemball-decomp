#include "CBulletManager.h"

#include "Multiplayer/Transport/CConnect.h"
#include "Gameplay/Objects/CGameObject.h"
#include "CBullet.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CBaseObjectManager.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/FixedPoint.h"

// FUNCTION: LEMBALL 0x00417d80
CBulletManager::CBulletManager()
	: CBaseObjectManager(NETWORK_OBJECT_MANAGER_MESSAGE_ID_BASE + OBJECT_MANAGER_TRANSPORT_BULLETS,
						 OBJECT_MANAGER_TRANSPORT_BULLETS)
{
	m_bullets = new CBullet[BULLET_ACTIVE_LIST_CAPACITY];
	for (int i = 0; i < BULLET_ACTIVE_LIST_CAPACITY; i++) {
		m_activeBullets[i] = NULL;
		m_bullets[i].SetId(CGameObject::NextLoadingId());
		m_bullets[i].m_manager = this;
	}
	if (g_pActiveConnection != NULL && g_pActiveConnection->m_isHost != 0) {
		m_poolStart = BULLET_OBJECT_POOL_PARTITION_CAPACITY;
		return;
	}
	m_poolStart = 0;
}

// FUNCTION: LEMBALL 0x00417e80
void CBulletManager::Restart()
{
	enum {
		BULLET_POOL_CAPACITY = sizeof(m_activeBullets) / sizeof(m_activeBullets[0])
	};
	m_iterator = 0;
	m_activeCount = 0;
	for (int i = 0; i < BULLET_POOL_CAPACITY; i++) {
		m_activeBullets[i] = NULL;
		m_bullets[i].Restart();
	}
}

// FUNCTION: LEMBALL 0x00417ec0
CBulletManager::~CBulletManager()
{
	delete[] m_bullets;
}

// FUNCTION: LEMBALL 0x00417ee0
CBullet* CBulletManager::NextFreeBullet()
{
	int i = 0;
	while (1) {
		if (i >= BULLET_OBJECT_POOL_PARTITION_CAPACITY) {
			return NULL;
		}
		if (m_bullets[m_poolStart + i].m_active == 0) {
			break;
		}
		i++;
	}
	return m_bullets + m_poolStart + i;
}

// FUNCTION: LEMBALL 0x00417f30
int CBulletManager::GetBulletCount()
{
	return m_activeCount;
}

// FUNCTION: LEMBALL 0x00417f40
CBullet* CBulletManager::GetFirstBullet()
{
	m_iterator = 0;
	return m_activeBullets[0];
}

// FUNCTION: LEMBALL 0x00417f50
CBullet* CBulletManager::GetNextBullet()
{
	int iterator = m_iterator + 1;
	m_iterator = iterator;
	if (m_activeCount <= iterator) {
		return NULL;
	}
	return m_activeBullets[iterator];
}

// FUNCTION: LEMBALL 0x00417f70
CBullet* CBulletManager::GetCurrentBullet()
{
	return m_activeBullets[m_iterator];
}

// FUNCTION: LEMBALL 0x00417f80
void CBulletManager::RequestRemoteBullet(CBullet* p_bullet)
{
	m_activeBullets[m_activeCount] = p_bullet;
	m_activeCount = m_activeCount + 1;
}

// FUNCTION: LEMBALL 0x00417fa0
bool CBulletManager::RequestBullet(unsigned short p_id,
								   eBulletType p_bulletType,
								   eOwner p_owner,
								   int p_sourceObjectId,
								   AICOORD p_start,
								   AICOORD p_target)
{
	if (m_activeCount < BULLET_ACTIVE_LIST_CAPACITY) {
		m_activeBullets[m_activeCount] = NextFreeBullet();
		if (m_activeBullets[m_activeCount] != NULL) {
			m_activeBullets[m_activeCount]->Set(p_id, p_bulletType, p_owner, p_sourceObjectId, p_start, p_target);
			m_activeBullets[m_activeCount]->FireBullet();
			m_activeCount = m_activeCount + 1;
		}
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x00418040
void CBulletManager::Process()
{
	CBullet* bullet = GetFirstBullet();
	while (bullet != NULL) {
		if (bullet->Process() == 0) {
			RemoveBullet(bullet);
		}
		bullet = GetNextBullet();
	}
}

// FUNCTION: LEMBALL 0x00418080
void CBulletManager::RemoveBullet(CBullet* p_bullet)
{
	int i = 0;
	CBullet** entry = m_activeBullets;
	do {
		if (p_bullet == *entry) {
			CBullet** slot = &m_activeBullets[i];
			m_activeBullets[i]->Free();
			if (i < BULLET_ACTIVE_LIST_SEARCH_LAST_INDEX) {
				int count = BULLET_ACTIVE_LIST_SEARCH_LAST_INDEX - i;
				i += count;
				do {
					CBullet* next = *(slot + 1);
					slot++;
					slot[-1] = next;
				} while (--count);
			}
			m_activeBullets[i] = NULL;
			m_activeCount--;
			return;
		}
		entry++;
		i++;
	} while (i < BULLET_ACTIVE_LIST_SEARCH_COUNT);
}

// FUNCTION: LEMBALL 0x004180e0
int CBulletManager::GetViewData(CViewData* p_viewData)
{
	CViewData* viewData;
	CBullet* bullet = GetFirstBullet();
	int count = 0;
	if (bullet != NULL) {
		viewData = p_viewData;
		do {
			bullet->GetViewData(*viewData);
			viewData++;
			count++;
			bullet = GetNextBullet();
		} while (bullet != NULL);
	}
	return count;
}

// FUNCTION: LEMBALL 0x00418120
bool CBulletManager::CheckGroupIntersection(CVSRect* p_rect, AICOORD* p_coordinate)
{
	int rectTop;
	int rectLeft = p_rect->m_x;
	int rectRight = p_rect->m_width + rectLeft;
	rectTop = p_rect->m_y;
	int rectBottom = p_rect->m_height + rectTop;
	CBullet* bullet = GetFirstBullet();
	while (bullet != NULL) {
		int left = bullet->m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
		int top = bullet->m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
		int right = left + 8;
		int bottom = top + 8;
		left -= 8;
		top -= 8;
		if (rectRight > left && right > rectLeft && rectBottom > top && bottom > rectTop) {
			p_coordinate->m_xFixed = bullet->m_position.m_xFixed;
			p_coordinate->m_yFixed = bullet->m_position.m_yFixed;
			p_coordinate->m_zFixed = bullet->m_position.m_zFixed;
			return true;
		}
		bullet = GetNextBullet();
	}
	return false;
}
