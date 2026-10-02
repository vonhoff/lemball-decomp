#ifndef LEMBALL_AI_BASE_TENEMYLEMMINGUNION_H
#define LEMBALL_AI_BASE_TENEMYLEMMINGUNION_H

struct tagWaypointInformation;

// SIZE 0x04
union tEnemyLemmingUnion {
	tagWaypointInformation* m_waypointInformation;
};

#endif
