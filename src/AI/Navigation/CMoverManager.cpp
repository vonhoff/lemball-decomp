#include "CMoverManager.h"

#include "AI/Base/CGameObject.h"
#include "AI/Managers/CBaseObjectManager.h"
#include "CAI.h"
#include "CMover.h"

// FUNCTION: LEMBALL 0x0042f190
CMoverManager::CMoverManager(CAI* p_ai, int p_capacity) : CBaseObjectManager(0x1a, 0xf)
{
	m_ai = p_ai;
	m_capacity = p_capacity;
	m_movers = 0;
}

// FUNCTION: LEMBALL 0x0042f1f0
void CMoverManager::Restart()
{
	if (m_movers != 0) {
		for (int i = 0; i < m_capacity; i++) {
			m_movers[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x0042f220
void CMoverManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_movers = 0;
		return;
	}
	if (m_movers == 0) {
		m_movers = new CMover[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
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

// FUNCTION: LEMBALL 0x0042f2f0
CMover* CMoverManager::Find(int p_x, int p_y, int& p_height)
{
	for (int i = 0; i < m_count; i++) {
		if (m_movers[i].IsAt(p_x, p_y, p_height)) {
			return &m_movers[i];
		}
	}
	return 0;
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
	for (int i = 0; i < m_count; i++) {
		m_movers[i].GetViewData(*p_viewData++);
		count++;
	}
	return count;
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
void CMoverManager::Switch(int p_message, int p_id)
{
	CMoverManager* self = this;
	int index = 0;
	while (index < self->m_count) {
		if ((unsigned short) self->m_movers[index].GetId() == p_id) {
			if (p_message == 4) {
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
	unsigned short count = *(unsigned short*) p_data;
	p_data += 2;
	Initialise(count);
	m_count = 0;
	for (int i = 0; i < count; i++) {
		unsigned short id;
		if (m_ai->m_levelVersion > 1) {
			id = *(unsigned short*) p_data;
			p_data += 2;
		}
		else {
			id = (unsigned short) CGameObject::NextId();
		}

		int pathId = 0;
		int movementMode = 0;
		if (m_ai->m_levelVersion > 5) {
			pathId = *(unsigned short*) p_data;
			p_data += 2;
			if ((pathId & 0x8000) != 0) {
				movementMode = 1;
				pathId &= 0x7fff;
			}
		}

		int startNode = *(unsigned short*) p_data;
		p_data += 2;
		int nodeCount = *(unsigned short*) p_data;
		p_data += 2;
		Add(id, pathId, movementMode, startNode, nodeCount);
	}
}
