#include "CNode.h"

#include "AI/Base/CPt3.h"
#include "NodeNeighbour.h"

// FUNCTION: LEMBALL 0x004212e0
CNode::CNode()
{
	m_xFixed = 0xaa55aa55;
	m_yFixed = 0xaa55aa55;
	m_neighbours = 0;
}

// FUNCTION: LEMBALL 0x00421300
void CNode::Restart()
{
	m_neighbourCount = 0;
}

// FUNCTION: LEMBALL 0x00421310
CNode::~CNode()
{
	if (m_neighbours != 0) {
		delete[] m_neighbours;
	}
}

// FUNCTION: LEMBALL 0x00421330
void CNode::Initialise(int p_x, int p_y, int p_neighbourCapacity)
{
	if (p_neighbourCapacity != 0 && m_neighbours != 0) {
		m_neighbours = (NodeNeighbour*) operator new(p_neighbourCapacity * sizeof(NodeNeighbour));
	}
	m_neighbourCapacity = p_neighbourCapacity;
	m_xFixed = p_x << 12;
	m_yFixed = p_y << 12;
}

// FUNCTION: LEMBALL 0x00421370
void CNode::SetFixedCoordinates(int p_x, int p_y, unsigned int p_unused)
{
	m_xFixed = p_x;
	m_yFixed = p_y;
}

// FUNCTION: LEMBALL 0x00421380
void CNode::SetIntegerCoordinates(int p_x, int p_y)
{
	m_xFixed = p_x << 12;
	m_yFixed = p_y << 12;
}

// FUNCTION: LEMBALL 0x004213a0
void CNode::AddANeighbour(int p_node, int p_cost)
{
	m_neighbours[m_neighbourCount].m_node = p_node;
	m_neighbours[m_neighbourCount].m_cost = p_cost;
	m_neighbourCount++;
}

// FUNCTION: LEMBALL 0x004213d0
CPt3 CNode::Position()
{
	return CPt3(m_xFixed, m_yFixed, 0xaa55aa55);
}

// FUNCTION: LEMBALL 0x004213f0
void CNode::ExtractIntegerPosition(int* p_x, int* p_y)
{
	*p_x = m_xFixed >> 12;
	*p_y = m_yFixed >> 12;
}

// FUNCTION: LEMBALL 0x00421410
void CNode::GetFixedPosition(int* p_coordinates)
{
	p_coordinates[0] = m_xFixed;
	p_coordinates[1] = m_yFixed;
}
