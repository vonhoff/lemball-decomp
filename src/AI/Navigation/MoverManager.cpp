#include "MoverManager.h"

#include "Ai.h"
#include "Mover.h"

// 68K 0x10617f0e __ct__13CMoverManagerFP3CAIi
// FUNCTION: LEMBALL 0x0042f190
MoverManager::MoverManager(Ai* p_arg0, int p_arg1) : BaseObjectManager(0x1a, 0xf)
{
	m_ai = p_arg0;
	m_capacity = p_arg1;
	m_movers = 0;
}

// 68K 0x10617f9a Restart__13CMoverManagerFv
// STUB: LEMBALL 0x0042f1f0
void MoverManager::Restart()
{
}

// 68K 0x10617ffe Initialise__13CMoverManagerFi
// FUNCTION: LEMBALL 0x0042f220
void MoverManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_movers = 0;
		return;
	}
	if (m_movers == 0) {
		m_movers = new Mover[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_movers[i].Restart();
			m_movers[i].m_manager = this;
		}
	}
}

// 68K 0x106180ca __dt__13CMoverManagerFv
// FUNCTION: LEMBALL 0x0042f2c0
MoverManager::~MoverManager()
{
	delete[] m_movers;
}

// 68K 0x10618140 Find__13CMoverManagerFiiRi
// STUB: LEMBALL 0x0042f2f0
Mover* MoverManager::Find(int p_x, int p_y, int& p_height)
{
	return 0;
}

// 68K 0x106181c2 Process__13CMoverManagerFv
// STUB: LEMBALL 0x0042f500
void MoverManager::Process()
{
}

// 68K 0x10618236 GetViewData__13CMoverManagerFP9CViewData
// STUB: LEMBALL 0x0042f540
int MoverManager::GetViewData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x106182b4 Add__13CMoverManagerFUsiUcii
// STUB: LEMBALL 0x0042f5e0
void MoverManager::Add(unsigned short p_id,
					   int p_pathId,
					   unsigned char p_movementMode,
					   int p_startNode,
					   int p_nodeCount)
{
}

// 68K 0x10618322 Switch__13CMoverManagerF9swMessagei
// STUB: LEMBALL 0x0042f620
void MoverManager::Switch(int p_message, int p_id)
{
}

// 68K 0x106183b8 LoadLevel__13CMoverManagerFPUciUc
// FUNCTION: LEMBALL 0x0042f680
void MoverManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
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
			id = (unsigned short) GameObject::NextId();
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
