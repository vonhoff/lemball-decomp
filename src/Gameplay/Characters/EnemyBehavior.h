#ifndef LEMBALL_AI_BASE_ENEMYSTATES_H
#define LEMBALL_AI_BASE_ENEMYSTATES_H

enum eEnemyStateActions {
	ENEMY_ACTION_STOP = 0,
	ENEMY_ACTION_PATROL = 1,
	ENEMY_ACTION_TURN_AND_FIRE_RAPID = 2,
	ENEMY_ACTION_TURN_AND_FIRE_SLOW = 3,
	ENEMY_ACTION_TURN_AND_FIRE_RANDOM = 4
};

enum eEnemyStateRules {
	ENEMY_RULE_NONE = 0,
	ENEMY_RULE_RADIUS50 = 2,
	ENEMY_RULE_NOT_RADIUS50 = 3,
	ENEMY_RULE_RADIUS50_AND_LOS = 4,
	ENEMY_RULE_NOT_RADIUS50_AND_LOS = 5
};

enum eWaypointPatrolMode {
	WAYPOINT_PATROL_REVERSE = 0,
	WAYPOINT_PATROL_LOOP = 1
};

// SIZE 0x14
struct tagWaypointInformation {
	eWaypointPatrolMode m_patrolMode; // 0x00
	unsigned int m_waypointCount;     // 0x04
	int m_waypointIndex;              // 0x08
	int m_waypointStep;               // 0x0c
	unsigned short* m_waypoints;      // 0x10
};

// SIZE 0x04
union tEnemyLemmingUnion {
	tagWaypointInformation* m_waypointInformation;
};

#endif
