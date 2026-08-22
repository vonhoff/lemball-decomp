#ifndef LEMBALL_AI_NAVIGATION_NODE_H
#define LEMBALL_AI_NAVIGATION_NODE_H

#include "../../Common.h"
#include "../Base/Pt3.h" // complete type

// SIZE 0x14
class Node {
public:
	Node();
	Pt3 Position();
	void AddANeighbour(int p_node, int p_cost);
	void Initialise(int p_x, int p_y, int p_neighbourCapacity);
	void Restart();
	~Node();

private:
	int m_xFixed;                // 0x00
	int m_yFixed;                // 0x04
	int m_neighbourCapacity;     // 0x08
	NodeNeighbour* m_neighbours; // 0x0c
	int m_neighbourCount;        // 0x10
};

#endif
