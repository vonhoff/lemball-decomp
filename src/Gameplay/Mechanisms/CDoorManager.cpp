#include "CDoorManager.h"

#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Simulation/CAI.h"
#include "CDoor.h"
#include "Gameplay/Objects/CViewData.h"
#include "Level/LevelFormat.h"
#include "Gameplay/Objects/ObjectIds.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Gameplay/Objects/CBaseObjectManager.h"
#include "SwitchEntry.h"

class AICOORD;

// GLOBAL: LEMBALL 0x0049cf48
unsigned short g_wNextDoorIndex = 0;

// FUNCTION: LEMBALL 0x0040df30
CDoorManager::CDoorManager(CAI* p_ai, int p_capacity)
	: CBaseObjectManager(NETWORK_OBJECT_MANAGER_MESSAGE_ID_BASE + OBJECT_MANAGER_TRANSPORT_DOORS,
						 OBJECT_MANAGER_TRANSPORT_DOORS)
{
	m_ai = p_ai;
	m_capacity = p_capacity;
	m_doors = NULL;
}

// FUNCTION: LEMBALL 0x0040df90
void CDoorManager::Restart()
{
	if (m_doors != NULL) {
		for (int i = 0; i < m_capacity; i++) {
			m_doors[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x0040dfc0
void CDoorManager::Initialise(int p_capacity)
{
	g_wNextDoorIndex = 0;
	m_count = 0;
	m_capacity = p_capacity;
	if (p_capacity == 0) {
		m_doors = NULL;
		return;
	}
	if (m_doors == NULL) {
		m_doors = new CDoor[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_doors[i].Restart();
			m_doors[i].m_manager = this;
		}
	}
}

// FUNCTION: LEMBALL 0x0040e060
CDoorManager::~CDoorManager()
{
	if (m_doors != NULL) {
		delete[] m_doors;
	}
}

// FUNCTION: LEMBALL 0x0040e080
int CDoorManager::GetViewData(CViewData* p_viewData)
{
	for (int i = 0; i < m_count; i++) {
		m_doors[i].GetViewData(p_viewData[i]);
	}
	return m_count;
}

// FUNCTION: LEMBALL 0x0040e0c0
int CDoorManager::Add(unsigned short p_id,
					  eObjectType p_objectType,
					  unsigned short p_doorType,
					  int p_x,
					  int p_y,
					  int p_z)
{
	if (m_count < m_capacity) {
		if (p_id == INVALID_OBJECT_ID) {
			p_id = (unsigned short) CGameObject::NextId();
		}
		m_doors[m_count].SetId(p_id);
		m_doors[m_count].Set(p_objectType, p_doorType, p_x, p_y, p_z);
		m_count++;
		return m_count - 1;
	}
	return INVALID_DOOR_INDEX;
}

// FUNCTION: LEMBALL 0x0040e140
void CDoorManager::RemoveDoorByObject(CDoor* p_door)
{
	unsigned short objectId = (unsigned short) p_door->GetId();
	for (int index = 0; index < m_count; index++) {
		if ((unsigned short) m_doors[index].GetId() == objectId) {
			m_doors[index].Delete();
			m_doors[index].SetId(INVALID_OBJECT_ID);
			for (int next = index + 1; next < m_count; next++) {
				m_doors[next - 1] = m_doors[next];
			}
			m_count--;
			return;
		}
	}
}

// FUNCTION: LEMBALL 0x0040e500
int CDoorManager::Open(const AICOORD& p_position, CGameObject* p_object)
{
	int i = 0;
	if (0 < m_count) {
		while (true) {
			if (m_doors[i].Hits(p_position, p_object)) {
				return 1;
			}
			i++;
			if (m_count <= i) {
				break;
			}
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0040e550
void CDoorManager::Process()
{
	for (int i = 0; i < m_count; i++) {
		m_doors[i].m_requestEnabled = 1;
		if (m_doors[i].m_activationPending != 0 || m_doors[i].m_isRemoteObject != 0) {
			m_doors[i].Process();
		}
	}
}

// FUNCTION: LEMBALL 0x0040e5a0
void CDoorManager::Switch(swMessage p_message, int p_id)
{
	int i = 0;
	if (0 < m_count) {
		while (true) {
			if ((unsigned short) m_doors[i].GetId() == p_id) {
				break;
			}
			i++;
			if (m_count <= i) {
				return;
			}
		}
		if (p_message == SW_DOOR) {
			m_doors[i].Unlock();
		}
	}
}

// FUNCTION: LEMBALL 0x0040e600
unsigned short CDoorManager::Id(int p_index)
{
	if (p_index < m_count) {
		return m_doors[p_index].GetId();
	}
	return INVALID_OBJECT_ID;
}

// FUNCTION: LEMBALL 0x0040e630
void CDoorManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short* data;
	unsigned short count;

	data = (unsigned short*) p_data;
	count = *data++;
	unsigned int capacity = count;
	Initialise(capacity);
	if (count != 0) {
		unsigned int remaining = capacity;
		unsigned short id;
		eObjectType objectType;
		unsigned short doorType;
		int x;
		int y;
		int z;
		do {
			if (m_ai->m_levelVersion > LEVEL_VERSION_LAST_WITHOUT_OBJECT_IDS) {
				id = *data++;
			}
			else {
				id = (unsigned short) CGameObject::NextId();
			}
			objectType = (eObjectType) *data++;
			if (m_ai->m_levelVersion > LEVEL_VERSION_LAST_WITHOUT_DOOR_TYPES) {
				doorType = *data++;
			}
			else {
				doorType = 0;
			}
			x = *data++;
			y = *data++;
			z = *data++;
			Add(id, objectType, doorType, x, y, z);
			remaining--;
		} while (remaining != 0);
	}
}
