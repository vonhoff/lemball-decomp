#include "CNodeManager.h"

#include "AI/Base/CPt3.h"
#include "CNode.h"

// FUNCTION: LEMBALL 0x00421180
CNodeManager::CNodeManager(int p_capacity)
{
	m_nodes = 0;
	m_capacity = p_capacity;
}

// FUNCTION: LEMBALL 0x004211a0
void CNodeManager::Restart()
{
	int i = 0;
	if (m_nodes != 0 && i < m_capacity) {
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
		m_nodes = 0;
		return;
	}
	if (m_nodes == 0) {
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
	if (m_nodes != 0) {
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
void CNodeManager::LoadLevel(unsigned char* p_data, unsigned int p_dataSize, unsigned int p_skip)
{
	unsigned char* end = p_data + p_dataSize;
	int count = *(unsigned short*) p_data;
	p_data += 2;

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
		x = ((unsigned short*) p_data)[0];
		y = ((unsigned short*) p_data)[1];
		neighbourCount = ((unsigned short*) p_data)[2];

		m_nodes[i].Initialise(x, y, neighbourCount);
		p_data += 6;
		if (neighbourCount > 0) {
			do {
				m_nodes[i].AddANeighbour(((unsigned short*) p_data)[0], ((unsigned short*) p_data)[1]);
				p_data += 4;
				neighbourCount--;
			} while (neighbourCount != 0);
		}
		i++;
	} while (p_data < end);
}
