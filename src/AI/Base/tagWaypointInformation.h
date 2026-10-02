#ifndef LEMBALL_AI_BASE_TAGWAYPOINTINFORMATION_H
#define LEMBALL_AI_BASE_TAGWAYPOINTINFORMATION_H

enum eWaypointPatrolMode {
	WAYPOINT_PATROL_REVERSE = 0,
	WAYPOINT_PATROL_LOOP = 1
};

// SIZE 0x14
struct tagWaypointInformation {
	unsigned int m_patrolMode;    // 0x00
	unsigned int m_waypointCount; // 0x04
	unsigned int m_waypointIndex; // 0x08
	int m_waypointStep;           // 0x0c
	unsigned short* m_waypoints;  // 0x10
};

#endif
