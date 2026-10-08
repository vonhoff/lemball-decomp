#include "CMoverManager.h"

#include "CMover.h"
#include "Gameplay/Mechanisms/SwitchEntry.h"
#include "Gameplay/Objects/CBaseObjectManager.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/ObjectIds.h"
#include "Gameplay/Simulation/CAI.h"
#include "Level/LevelFormat.h"

#include <stddef.h>

enum {
	MOVER_PATH_WAIT_FOR_SWITCH_FLAG = 0x8000,
	MOVER_PATH_ID_MASK = 0x7fff
};

// FUNCTION: LEMBALL 0x0042f190
CMoverManager::CMoverManager(CAI* p_ai, int p_capacity)
	: CBaseObjectManager(NETWORK_OBJECT_MANAGER_MESSAGE_ID_BASE + OBJECT_MANAGER_TRANSPORT_MOVERS,
						 OBJECT_MANAGER_TRANSPORT_MOVERS)
{
	m_ai = p_ai;
	m_capacity = p_capacity;
	m_movers = NULL;
}

// FUNCTION: LEMBALL 0x0042f1f0
void CMoverManager::Restart()
{
	if (m_movers != NULL) {
		for (int i = 0; i < m_capacity; i++) {
			m_movers[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x0042f220
void CMoverManager::Initialise(int p_capacity)
{
	int i;
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_movers = NULL;
		return;
	}
	if (m_movers == NULL) {
		m_movers = new CMover[p_capacity];
		for (i = 0; i < m_capacity; i++) {
			m_movers[i].Restart();
			m_movers[i].m_manager = this;
		}
	}
}

// FUNCTION: LEMBALL 0x0042f2c0
CMoverManager::~CMoverManager()
{
	delete[] m_movers;
}

// FUNCTION: LEMBALL 0x0042f2e0
void CMoverManager::ResetCount()
{
	m_count = 0;
}

// FUNCTION: LEMBALL 0x0042f2f0
CMover* CMoverManager::Find(int p_x, int p_y, int& p_height)
{
	for (int i = 0; i < m_count; i++) {
		if (m_movers[i].IsAt(p_x, p_y, p_height)) {
			return &m_movers[i];
		}
	}
	return NULL;
}

// FUNCTION: LEMBALL 0x0042f350
void CMoverManager::RemoveMover(CMover* p_mover)
{
	int count;
	int index = 0;
	count = m_count;
	if (index < count) {
		while (p_mover != &m_movers[index]) {
			index++;
			if (index >= count) {
				return;
			}
		}
		m_movers[index].SetId(INVALID_OBJECT_ID);
		for (index++; index < m_count; index++) {
			CMover* destination;
			CMover* source;
			source = &m_movers[index];
			destination = source - 1;
			*destination = *source;
		}
		m_count--;
	}
}

// FUNCTION: LEMBALL 0x0042f500
void CMoverManager::Process()
{
	for (int i = 0; i < m_count; i++) {
		if (m_movers[i].m_active != 0) {
			m_movers[i].Process();
		}
	}
}

// FUNCTION: LEMBALL 0x0042f540
int CMoverManager::GetViewData(CViewData* p_viewData)
{
	int count = 0;
	int i = 0;
	if (i < m_count) {
		CViewData* output = p_viewData;
		do {
			m_movers[i].GetViewData(*output++);
			count++;
			i++;
		} while (i < m_count);
	}
	return count;
}

// FUNCTION: LEMBALL 0x0042f590
bool CMoverManager::AnyMoverMatches(unsigned int p_arg0, unsigned int p_arg1)
{
	int index = 0;
	if (index < m_count) {
		do {
			if (m_movers[index].AlwaysFalse(p_arg0, p_arg1)) {
				return true;
			}
			index++;
		} while (index < m_count);
	}
	return false;
}

// FUNCTION: LEMBALL 0x0042f5e0
void CMoverManager::Add(unsigned short p_id,
						int p_pathId,
						unsigned int p_movementMode,
						int p_startNode,
						int p_nodeCount)
{
	if (m_count < m_capacity) {
		m_movers[m_count].Set(p_id, p_pathId, p_movementMode, p_startNode, p_nodeCount);
		m_count++;
	}
}

// FUNCTION: LEMBALL 0x0042f620
void CMoverManager::Switch(swMessage p_message, int p_id)
{
	CMoverManager* self = this;
	int index = 0;
	while (index < self->m_count) {
		if ((unsigned short) self->m_movers[index].GetId() == p_id) {
			if (p_message == SW_MOVER) {
				self->m_movers[index].Switch();
			}
			return;
		}
		index++;
	}
}

// FUNCTION: LEMBALL 0x0042f680
void CMoverManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned char* cursor = p_data;
	unsigned short count = *(unsigned short*) cursor;
	cursor += 2;
	Initialise(count);
	m_count = 0;
	for (int i = 0; i < count; i++) {
		unsigned short id;
		if (m_ai->m_levelVersion > LEVEL_VERSION_LAST_WITHOUT_OBJECT_IDS) {
			id = *(unsigned short*) cursor;
			cursor += 2;
		}
		else {
			id = (unsigned short) CGameObject::NextId();
		}

		int pathId = 0;
		int movementMode = 0;
		if (m_ai->m_levelVersion > LEVEL_VERSION_LAST_WITHOUT_MOVER_PATHS) {
			pathId = *(unsigned short*) cursor;
			cursor += 2;
			if ((pathId & MOVER_PATH_WAIT_FOR_SWITCH_FLAG) != 0) {
				movementMode = 1;
				pathId &= MOVER_PATH_ID_MASK;
			}
		}

		int startNode = *(unsigned short*) cursor;
		cursor += 2;
		int nodeCount = *(unsigned short*) cursor;
		cursor += 2;
		Add(id, pathId, movementMode, startNode, nodeCount);
	}
}
