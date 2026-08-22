#ifndef LEMBALL_SCAFFOLD_AI_BASE_WAYPOINTINFORMATION_H
#define LEMBALL_SCAFFOLD_AI_BASE_WAYPOINTINFORMATION_H

#include "../../Common.h"

// SIZE 0x14
struct WaypointInformation {
public:
	// No functions mapped yet.

	unsigned int m_action;        // 0x00
	unsigned int m_waypointCount; // 0x04
	unsigned int m_value;         // 0x08
	int m_signedValue;            // 0x0c
	unsigned short* m_waypoints;  // 0x10
};

#endif
