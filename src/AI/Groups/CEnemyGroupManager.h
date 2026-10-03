#ifndef LEMBALL_AI_GROUPS_CENEMYGROUPMANAGER_H
#define LEMBALL_AI_GROUPS_CENEMYGROUPMANAGER_H

#include "../Base/EnemyStates.h"
#include "CGenericGroupManager.h"

class CAI;
class CFormationManager;
class CEnemyGroup;
class CObjectManager;
struct tagLoadEnemyData;
struct tagLoadEnemyDataAdditionalAction;
struct tagWaypointInformation;
// SIZE 0xb0
// VTABLE: LEMBALL 0x004953f8
class CEnemyGroupManager : public CGenericGroupManager {
public:
	CEnemyGroupManager(CAI* p_ai, CObjectManager* p_objectManager, CFormationManager* p_formationManager);
	tagLoadEnemyDataAdditionalAction* LoadLevelAdditional_Waypoint(tagLoadEnemyDataAdditionalAction* p_data,
																   tagWaypointInformation*& p_waypointInfo);
	void LoadLevel(tagLoadEnemyData* p_data, unsigned long p_dataSize, unsigned int p_skip);
	void Restart();
	void RemoveEnemyGroup(CEnemyGroup* p_group);
	void AddEnemyGroup(int p_x,
					   int p_y,
					   int p_z,
					   eEnemyStateActions p_action0,
					   eEnemyStateRules p_rule0,
					   eEnemyStateActions p_action1,
					   eEnemyStateRules p_rule1,
					   eEnemyStateActions p_action2,
					   eEnemyStateRules p_rule2,
					   unsigned short p_waypointStart,
					   int p_waypointCount);
};

unsigned long ENEMY_GetLONG(unsigned long* p_data);
#endif
