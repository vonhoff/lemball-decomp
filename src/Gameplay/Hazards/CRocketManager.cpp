#include "CRocketManager.h"

#include "Gameplay/Simulation/CAI.h"
#include "CRocket.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Level/LevelFormat.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/CBaseObjectManager.h"

// FUNCTION: LEMBALL 0x00426ac0
CRocketManager::CRocketManager(CAI* p_ai, int p_capacity)
	: CBaseObjectManager(NETWORK_OBJECT_MANAGER_MESSAGE_ID_BASE + OBJECT_MANAGER_TRANSPORT_ROCKETS,
						 OBJECT_MANAGER_TRANSPORT_ROCKETS)
{
	m_ai = p_ai;
	m_capacity = p_capacity;
	m_rockets = NULL;
}

// FUNCTION: LEMBALL 0x00426b20
void CRocketManager::Restart()
{
	if (m_rockets != NULL) {
		for (int i = 0; i < m_capacity; i++) {
			m_rockets[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x00426b50
void CRocketManager::Initialise(int p_capacity)
{
	int i;
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_rockets = NULL;
		return;
	}
	if (m_rockets == NULL) {
		m_rockets = new CRocket[p_capacity];
		for (i = 0; i < m_capacity; i++) {
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

// FUNCTION: LEMBALL 0x00426c30
void CRocketManager::RemoveRocket(CRocket* p_rocket)
{
	for (int index = 0; index < m_count; index++) {
		if (&m_rockets[index] == p_rocket) {
			m_rockets[index].SetId(INVALID_OBJECT_ID);
			for (int next = index + 1; next < m_count; next++) {
				m_rockets[next - 1] = m_rockets[next];
			}
			m_count--;
			return;
		}
	}
}

// FUNCTION: LEMBALL 0x00426fb0
int CRocketManager::StepOn(const AICOORD& p_position, CGameObject* p_object)
{
	int i = 0;
	while (true) {
		if (m_count <= i) {
			return 0;
		}
		CRocket* rocket = &m_rockets[i];
		if (rocket->m_active != 0 && rocket->m_action == ACTION_READY && rocket->m_requestedAction == ACTION_READY &&
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
	int ordinal = 0;
	int count = 0;
	if (m_count > ordinal) {
		CViewData* viewData = p_viewData;
		do {
			CRocket* rocket = &m_rockets[ordinal];
			if (rocket->m_action != ACTION_READY) {
				rocket->GetViewData(*viewData++);
				count++;
			}
			ordinal++;
		} while (m_count > ordinal);
	}
	return count;
}

// FUNCTION: LEMBALL 0x004270b0
void CRocketManager::Add(unsigned short p_id, int p_x, int p_y, int p_z)
{
	if (m_count < m_capacity) {
		AICOORD position;
		position.m_xFixed = p_x << FIXED_POINT_FRACTION_BITS;
		position.m_yFixed = p_y << FIXED_POINT_FRACTION_BITS;
		position.m_zFixed = p_z << FIXED_POINT_FRACTION_BITS;
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
			if (m_ai->m_levelVersion > LEVEL_VERSION_LAST_WITHOUT_OBJECT_IDS) {
				id = *(unsigned short*) p_data;
				p_data += 2;
			}
			else {
				id = (unsigned short) CGameObject::NextId();
			}
			struct LevelPosition {
				unsigned short x;
				unsigned short y;
				unsigned short z;
			} position;
			position.x = *(unsigned short*) p_data;
			p_data += sizeof(position.x);
			position.y = *(unsigned short*) p_data;
			p_data += sizeof(position.y);
			position.z = *(unsigned short*) p_data;
			p_data += sizeof(position.z);
			Add(id, position.x, position.y, position.z);
			remaining--;
		} while (remaining != 0);
	}
}
