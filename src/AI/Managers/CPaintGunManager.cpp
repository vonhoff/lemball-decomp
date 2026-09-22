#include "CPaintGunManager.h"

#include "../Navigation/CAi.h"
#include "../Objects/CPaintGun.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Managers/CBaseObjectManager.h"

// FUNCTION: LEMBALL 0x0042bfe0
CPaintGunManager::CPaintGunManager(CAi* p_ai, int p_capacity) : CBaseObjectManager(0x1f, 0x14)
{
	m_ai = p_ai;
	m_capacity = p_capacity;
	m_paintGuns = 0;
}

// FUNCTION: LEMBALL 0x0042c040
void CPaintGunManager::Restart()
{
	if (m_paintGuns != 0) {
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
		m_paintGuns = 0;
		return;
	}
	if (m_paintGuns == 0) {
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
	for (int i = 0; i < m_count; i++) {
		if (m_paintGuns[i].m_enabled != 0) {
			m_paintGuns[i].GetViewData(*p_viewData++);
			count++;
		}
	}
	return count;
}

// FUNCTION: LEMBALL 0x0042c590
void CPaintGunManager::Add(unsigned short p_id, int p_x, int p_y, int p_z, int p_direction)
{
	if (m_count < m_capacity) {
		AiCoord position;
		position.m_xFixed = p_x << 12;
		position.m_yFixed = p_y << 12;
		position.m_zFixed = p_z << 12;
		m_paintGuns[m_count].Set(p_id, position, 0);
		m_paintGuns[m_count].m_direction = p_direction;
		m_count++;
	}
}

// FUNCTION: LEMBALL 0x0042c610
void CPaintGunManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short count = *(unsigned short*) p_data;
	p_data += 2;
	unsigned int remaining = count;
	Initialise(remaining);
	if (count != 0) {
		unsigned short id;
		unsigned short x;
		unsigned short y;
		unsigned short z;
		unsigned short direction;
		do {
			if (m_ai->m_levelVersion > 1) {
				id = *(unsigned short*) p_data;
				p_data += 2;
			}
			else {
				id = (unsigned short) CGameObject::NextId();
			}
			x = *(unsigned short*) p_data;
			p_data += 2;
			y = *(unsigned short*) p_data;
			p_data += 2;
			z = *(unsigned short*) p_data;
			p_data += 2;
			direction = *(unsigned short*) p_data;
			p_data += 2;
			Add(id, x, y, z, direction);
			remaining--;
		} while (remaining != 0);
	}
}
