#include "DoorManager.h"

#include "../Base/GameObject.h"
#include "../Navigation/Ai.h"
#include "../Objects/Door.h"
#include "../Objects/ViewData.h"

// GLOBAL: LEMBALL 0x0049cf48
word g_wNextDoorIndex = 0;

// 68K 0x10606c72 __ct__12CDoorManagerFP3CAIi
// FUNCTION: LEMBALL 0x0040df30
DoorManager::DoorManager(Ai* p_arg0, int p_arg1) : BaseObjectManager(0x13, 0x8)
{
	m_ai = p_arg0;
	m_capacity = p_arg1;
	m_doors = 0;
}

// 68K 0x10606cfc Restart__12CDoorManagerFv
// FUNCTION: LEMBALL 0x0040df90
void DoorManager::Restart()
{
	if (m_doors != 0) {
		for (int i = 0; i < m_capacity; i++) {
			m_doors[i].Restart();
		}
	}
}

// 68K 0x10606d5e Initialise__12CDoorManagerFi
// FUNCTION: LEMBALL 0x0040dfc0
void DoorManager::Initialise(int p_capacity)
{
	g_wNextDoorIndex = 0;
	m_count = 0;
	m_capacity = p_capacity;
	if (p_capacity == 0) {
		m_doors = 0;
		return;
	}
	if (m_doors == 0) {
		m_doors = new Door[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_doors[i].Restart();
			m_doors[i].m_manager = this;
		}
	}
}

// 68K 0x10606e32 __dt__12CDoorManagerFv
// FUNCTION: LEMBALL 0x0040e060
DoorManager::~DoorManager()
{
	if (m_doors != 0) {
		delete[] m_doors;
	}
}

// 68K 0x10606eaa GetViewData__12CDoorManagerFP9CViewData
// FUNCTION: LEMBALL 0x0040e080
int DoorManager::GetViewData(ViewData* p_viewData)
{
	int count = 0;
	if (0 < m_count) {
		do {
			m_doors[count].GetViewData(*p_viewData);
			p_viewData++;
			count++;
		} while (count < m_count);
	}
	return m_count;
}

// 68K 0x10606f2a Add__12CDoorManagerFUs11eObjectTypeUsiii
// FUNCTION: LEMBALL 0x0040e0c0
int DoorManager::Add(unsigned short p_id,
					 eObjectType p_objectType,
					 unsigned short p_doorType,
					 int p_x,
					 int p_y,
					 int p_z)
{
	if (m_count < m_capacity) {
		if (p_id == 0xffff) {
			p_id = (unsigned short) GameObject::NextId();
		}
		m_doors[m_count].SetId(p_id);
		m_doors[m_count].Set(p_objectType, p_doorType, p_x, p_y, p_z);
		m_count++;
		return m_count - 1;
	}
	return 0xffff;
}

// 68K 0x10606fe4 Open__12CDoorManagerFRC7AICOORDP11CGameObject
// FUNCTION: LEMBALL 0x0040e500
int DoorManager::Open(const AiCoord& p_position, GameObject* p_object)
{
	if (0 < m_count) {
		for (int i = 0; i < m_count; i++) {
			if (m_doors[i].Hits(p_position, p_object) != 0) {
				return 1;
			}
		}
	}
	return 0;
}

// 68K 0x10607066 Process__12CDoorManagerFv
// FUNCTION: LEMBALL 0x0040e550
void DoorManager::Process()
{
	if (0 < m_count) {
		for (int i = 0; i < m_count; i++) {
			m_doors[i].m_requestEnabled = 1;
			if (m_doors[i].m_activationPending != 0 || m_doors[i].m_isRemoteObject != 0) {
				m_doors[i].Process();
			}
		}
	}
}

// 68K 0x10607100 Switch__12CDoorManagerF9swMessagei
// FUNCTION: LEMBALL 0x0040e5a0
void DoorManager::Switch(swMessage p_message, int p_id)
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
		if (p_message == 3) {
			m_doors[i].Unlock();
		}
	}
}

// 68K 0x10607194 Id__12CDoorManagerFi
// FUNCTION: LEMBALL 0x0040e600
unsigned short DoorManager::Id(int p_index)
{
	if (p_index < m_count) {
		return (unsigned short) m_doors[p_index].GetId();
	}
	return 0xffff;
}

// 68K 0x106071e4 LoadLevel__12CDoorManagerFPUciUc
// FUNCTION: LEMBALL 0x0040e630
void DoorManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short* data = (unsigned short*) p_data;
	unsigned short count = *data++;
	unsigned int remaining = count;
	Initialise(count);
	if (count != 0) {
		do {
			unsigned short id;
			if (m_ai->m_levelVersion > 1) {
				id = *data++;
			}
			else {
				id = (unsigned short) GameObject::NextId();
			}
			unsigned short doorType = 0;
			unsigned short objectType = *data++;
			if (m_ai->m_levelVersion > 2) {
				doorType = *data++;
			}
			int x = *data++;
			int y = *data++;
			int z = *data++;
			Add(id, (eObjectType) objectType, doorType, x, y, z);
			remaining--;
		} while (remaining != 0);
	}
}
