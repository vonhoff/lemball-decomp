#include "CNodeManager.h"

#include "CNode.h"
#include "Gameplay/Geometry/CPt3.h"
#include "Level/LevelFormat.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00421180
CNodeManager::CNodeManager(int p_capacity)
{
	m_nodes = NULL;
	m_capacity = p_capacity;
}

// FUNCTION: LEMBALL 0x004211a0
void CNodeManager::Restart()
{
	int i = 0;
	if (m_nodes != NULL && i < m_capacity) {
		do {
			m_nodes[i].Restart();
			i++;
		} while (i < m_capacity);
	}
}

// FUNCTION: LEMBALL 0x004211d0
void CNodeManager::Initialise(int p_count)
{
	m_count = 0;
	if (p_count == 0) {
		m_nodes = NULL;
		return;
	}
	if (m_nodes == NULL) {
		m_nodes = new CNode[m_capacity];

		int i = 0;
		if (m_capacity > 0) {
			do {
				m_nodes[i].Restart();
				i++;
			} while (i < m_capacity);
		}
	}
}

// FUNCTION: LEMBALL 0x00421260
CNodeManager::~CNodeManager()
{
	if (m_nodes != NULL) {
		delete[] m_nodes;
	}
}

// FUNCTION: LEMBALL 0x004212a0
CPt3 CNodeManager::GetNodePosition(int p_node)
{
	return m_nodes[p_node].Position();
}

// FUNCTION: LEMBALL 0x004212c0
void CNodeManager::GetNodeIntegerPosition(int p_node, int* p_x, int* p_y)
{
	m_nodes[p_node].ExtractIntegerPosition(p_x, p_y);
}

// FUNCTION: LEMBALL 0x00421440
int CNodeManager::AddNode(int p_x, int p_y)
{
	int index = m_count;
	CNode* node = m_nodes + index;
	++m_count;
	node->Initialise(p_x, p_y, 0);
	return index;
}

// FUNCTION: LEMBALL 0x00421470
void CNodeManager::LoadLevel(tagLoadNodeInformation* p_information, unsigned long p_dataSize, unsigned int p_skip)
{
	unsigned char* p_data = (unsigned char*) p_information;
	unsigned char* end = p_data + p_dataSize;
	int count = p_information->m_nodeCount;
	p_data += sizeof(*p_information);

	Initialise(count);
	m_count = count;

	if (p_skip != 0) {
		return;
	}
	if (p_data >= end) {
		return;
	}

	int i = 0;
	int x;
	int y;
	int neighbourCount;
	do {
		tagLoadNodeData* node = (tagLoadNodeData*) p_data;
		x = node->m_x;
		y = node->m_y;
		neighbourCount = node->m_neighbourCount;

		m_nodes[i].Initialise(x, y, neighbourCount);
		p_data += sizeof(*node);
		if (neighbourCount > 0) {
			do {
				tagLoadNodeNeighbour* neighbour = (tagLoadNodeNeighbour*) p_data;
				m_nodes[i].AddANeighbour(neighbour->m_node, neighbour->m_cost);
				p_data += sizeof(*neighbour);
				neighbourCount--;
			} while (neighbourCount != 0);
		}
		i++;
	} while (p_data < end);
}
