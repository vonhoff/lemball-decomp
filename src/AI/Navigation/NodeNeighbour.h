#ifndef LEMBALL_AI_NAVIGATION_NODENEIGHBOUR_H
#define LEMBALL_AI_NAVIGATION_NODENEIGHBOUR_H

#include "../../Common.h"

// SIZE 0x08
struct NodeNeighbour {
	int m_node; // 0x00
	int m_cost; // 0x04
};

#endif
