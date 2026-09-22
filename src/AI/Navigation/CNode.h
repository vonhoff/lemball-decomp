#ifndef LEMBALL_AI_NAVIGATION_CNODE_H
#define LEMBALL_AI_NAVIGATION_CNODE_H

#include "../Base/CPt3.h" // complete type

struct NodeNeighbour;
// SIZE 0x14
class CNode {
public:
	CNode();
	CPt3 Position();
	void ExtractIntegerPosition(int* p_x, int* p_y);
	void AddANeighbour(int p_node, int p_cost);
	void Initialise(int p_x, int p_y, int p_neighbourCapacity);
	void Restart();
	void GetFixedPosition(int* p_coordinates);
	void SetFixedCoordinates(int p_x, int p_y, unsigned int p_unused);
	void SetIntegerCoordinates(int p_x, int p_y);
	~CNode();

private:
	int m_xFixed;                // 0x00
	int m_yFixed;                // 0x04
	int m_neighbourCapacity;     // 0x08
	NodeNeighbour* m_neighbours; // 0x0c
	int m_neighbourCount;        // 0x10
};

// SYNTHETIC: LEMBALL 0x004215e0
// CNode::`vector deleting destructor'

#endif
