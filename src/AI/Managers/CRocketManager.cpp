#include "CRocketManager.h"

#include "../Navigation/CAi.h"
#include "../Objects/CRocket.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Managers/CBaseObjectManager.h"

// FUNCTION: LEMBALL 0x00426ac0
CRocketManager::CRocketManager(CAi* p_ai, int p_capacity) : CBaseObjectManager(0x14, 9)
{
	m_ai = p_ai;
	m_capacity = p_capacity;
	m_rockets = 0;
}

// FUNCTION: LEMBALL 0x00426b20
void CRocketManager::Restart()
{
	if (m_rockets != 0) {
		for (int i = 0; i < m_capacity; i++) {
			m_rockets[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x00426b50
void CRocketManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_rockets = 0;
		return;
	}
	if (m_rockets == 0) {
		m_rockets = new CRocket[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_rockets[i].Restart();
			m_rockets[i].m_manager = this;
		}
	}
}

// FUNCTION: LEMBALL 0x00426c00
CRocketManager::~CRocketManager()
{
	delete[] m_rockets;
}

// FUNCTION: LEMBALL 0x00426c20
void CRocketManager::ResetCount()
{
	m_count = 0;
}

// FUNCTION: LEMBALL 0x00426fb0
int CRocketManager::StepOn(const AiCoord& p_position, CGameObject* p_object)
{
	int i = 0;
	while (true) {
		if (m_count <= i) {
			return 0;
		}
		CRocket* rocket = &m_rockets[i];
		if (rocket->m_active != 0 && rocket->m_action == ACTION_0x18 && rocket->m_requestedAction == ACTION_0x18 &&
			rocket->StepOn(p_position, p_object) != 0) {
			return 1;
		}
		i++;
	}
}

// FUNCTION: LEMBALL 0x00427010
void CRocketManager::Process()
{
	for (int i = 0; i < m_count; i++) {
		m_rockets[i].m_requestEnabled = 1;
		m_rockets[i].Process();
	}
}

// FUNCTION: LEMBALL 0x00427050
int CRocketManager::GetViewData(CViewData* p_viewData)
{
	int count = 0;
	for (int i = 0; i < m_count; i++) {
		if (m_rockets[i].m_action != ACTION_0x18) {
			m_rockets[i].GetViewData(*p_viewData++);
			count++;
		}
	}
	return count;
}

// FUNCTION: LEMBALL 0x004270b0
void CRocketManager::Add(unsigned short p_id, int p_x, int p_y, int p_z)
{
	if (m_count < m_capacity) {
		AiCoord position(p_x << 12, p_y << 12, p_z << 12);
		m_rockets[m_count].Set(p_id, position);
		m_count++;
	}
}

// FUNCTION: LEMBALL 0x00427110
void CRocketManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short count = *(unsigned short*) p_data;
	p_data += 2;
	unsigned int remaining = count;
	Initialise(remaining);
	if (count != 0) {
		do {
			unsigned short id;
			if (m_ai->m_levelVersion > 1) {
				id = *(unsigned short*) p_data;
				p_data += 2;
			}
			else {
				id = (unsigned short) CGameObject::NextId();
			}
			unsigned short x = *(unsigned short*) p_data;
			p_data += 2;
			unsigned short y = *(unsigned short*) p_data;
			p_data += 2;
			unsigned short z = *(unsigned short*) p_data;
			p_data += 2;
			Add(id, x, y, z);
			remaining--;
		} while (remaining != 0);
	}
}
