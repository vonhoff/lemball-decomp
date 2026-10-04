#include "CPaintGunManager.h"

#include "../Navigation/CAI.h"
#include "../Objects/CPaintGun.h"
#include "AI/Base/AICOORD.h"
#include "AI/Base/CGameObject.h"
#include "AI/Managers/CBaseObjectManager.h"

// FUNCTION: LEMBALL 0x0042bfe0
CPaintGunManager::CPaintGunManager(CAI* p_ai, int p_capacity)
	: CBaseObjectManager(NETWORK_OBJECT_MANAGER_MESSAGE_ID_BASE + OBJECT_MANAGER_TRANSPORT_PAINT_GUNS,
						 OBJECT_MANAGER_TRANSPORT_PAINT_GUNS)
{
	m_ai = p_ai;
	m_capacity = p_capacity;
	m_paintGuns = NULL;
}

// FUNCTION: LEMBALL 0x0042c040
void CPaintGunManager::Restart()
{
	if (m_paintGuns != NULL) {
		for (int i = 0; i < m_capacity; i++) {
			m_paintGuns[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x0042c070
void CPaintGunManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_paintGuns = NULL;
		return;
	}
	if (m_paintGuns == NULL) {
		m_paintGuns = new CPaintGun[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_paintGuns[i].Restart();
			m_paintGuns[i].m_manager = this;
		}
	}
}

// FUNCTION: LEMBALL 0x0042c120
CPaintGunManager::~CPaintGunManager()
{
	delete[] m_paintGuns;
}

// FUNCTION: LEMBALL 0x0042c140
void CPaintGunManager::ResetCount()
{
	m_count = 0;
}

// FUNCTION: LEMBALL 0x0042c150
void CPaintGunManager::RemovePaintGun(CPaintGun* p_paintGun)
{
	int index = 0;
	if (0 < m_count) {
		while (p_paintGun != &m_paintGuns[index]) {
			index++;
			if (m_count <= index) {
				return;
			}
		}
		m_paintGuns[index].SetId(INVALID_OBJECT_ID);
		for (int next = index + 1; next < m_count; next++) {
			m_paintGuns[next - 1] = m_paintGuns[next];
		}
		m_count--;
	}
}

// FUNCTION: LEMBALL 0x0042c4d0
void CPaintGunManager::Process()
{
	for (int i = 0; i < m_count; i++) {
		m_paintGuns[i].m_requestEnabled = 1;
		if (m_paintGuns[i].m_enabled != 0) {
			m_paintGuns[i].Process();
		}
	}
}

// FUNCTION: LEMBALL 0x0042c520
int CPaintGunManager::GetViewData(CViewData* p_viewData)
{
	int count = 0;
	CViewData* viewData;
	CPaintGunManager* manager = this;
	int ordinal = 0;
	if (manager->m_count > 0) {
		viewData = p_viewData;
		do {
			CPaintGun* gun = &manager->m_paintGuns[ordinal];
			if (gun->m_enabled != 0) {
				gun->GetViewData(*viewData++);
				count++;
			}
			ordinal++;
		} while (manager->m_count > ordinal);
	}
	return count;
}

// FUNCTION: LEMBALL 0x0042c590
void CPaintGunManager::Add(unsigned short p_id, int p_x, int p_y, int p_z, int p_direction)
{
	if (m_count < m_capacity) {
		AICOORD position;
		position.m_xFixed = p_x << FIXED_POINT_FRACTION_BITS;
		position.m_yFixed = p_y << FIXED_POINT_FRACTION_BITS;
		position.m_zFixed = p_z << FIXED_POINT_FRACTION_BITS;
		m_paintGuns[m_count].Set(p_id, position, 0);
		m_paintGuns[m_count].m_direction = p_direction;
		m_count++;
	}
}
