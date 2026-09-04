#include "NodeManager.h"

#include "Node.h"

// 68K 0x106184ca __ct__12CNodeManagerFi
// FUNCTION: LEMBALL 0x00421180
NodeManager::NodeManager(int p_arg0)
{
	m_nodes = 0;
	m_capacity = p_arg0;
}

// 68K 0x106184f8 Restart__12CNodeManagerFv
// FUNCTION: LEMBALL 0x004211a0
void NodeManager::Restart()
{
	int i = 0;
	if (m_nodes != 0 && i < m_capacity) {
		do {
			m_nodes[i].Restart();
			i++;
		} while (i < m_capacity);
	}
}

// 68K 0x1061854c Initialise__12CNodeManagerFi
// STUB: LEMBALL 0x004211d0
void NodeManager::Initialise(int p_count)
{
}

// 68K 0x106185ee __dt__12CNodeManagerFv
// STUB: LEMBALL 0x00421260
NodeManager::~NodeManager()
{
}

// 68K 0x10618642 GetNodePosition__12CNodeManagerFi
// FUNCTION: LEMBALL 0x004212a0
Pt3 NodeManager::GetNodePosition(int p_node)
{
	return m_nodes[p_node].Position();
}

// 68K 0x10618854 LoadLevel__12CNodeManagerFP22tagLoadNodeInformationUlUc
// STUB: LEMBALL 0x00421470
void NodeManager::LoadLevel(unsigned char* p_data, unsigned int p_dataSize, unsigned char p_skip)
{
}
