#ifndef LEMBALL_AI_GROUPS_CENEMYGROUPMANAGER_H
#define LEMBALL_AI_GROUPS_CENEMYGROUPMANAGER_H

#include "CGenericGroupManager.h"

class CAI;
class CFormationManager;
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
};

unsigned long ENEMY_GetLONG(unsigned long* p_data);
#endif
