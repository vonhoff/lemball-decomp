#ifndef LEMBALL_SCAFFOLD_AI_GROUPS_ENEMYGROUPMANAGER_H
#define LEMBALL_SCAFFOLD_AI_GROUPS_ENEMYGROUPMANAGER_H

#include "../../Common.h"
#include "GenericGroupManager.h" // complete type

// SIZE 0xb0
// VTABLE: LEMBALL 0x004953f8
class EnemyGroupManager : public GenericGroupManager {
public:
	EnemyGroupManager(Ai* p_arg0, ObjectManager* p_arg1, FormationManager* p_arg2);
	LoadEnemyDataAdditionalAction* LoadLevelAdditionalWaypoint(LoadEnemyDataAdditionalAction* p_data,
															   WaypointInformation*& p_waypointInfo);
	void LoadLevel(LoadEnemyData* p_data, unsigned long p_dataSize, unsigned char p_skip);
	void Restart();
};

unsigned long EnemyGetLong(unsigned long* p_data);
#endif
