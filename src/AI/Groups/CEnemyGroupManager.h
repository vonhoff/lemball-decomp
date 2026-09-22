#ifndef LEMBALL_AI_GROUPS_CENEMYGROUPMANAGER_H
#define LEMBALL_AI_GROUPS_CENEMYGROUPMANAGER_H

#include "CGenericGroupManager.h" // complete type

class CAi;
class CFormationManager;
class CObjectManager;
struct LoadEnemyData;
struct LoadEnemyDataAdditionalAction;
struct WaypointInformation;
// SIZE 0xb0
// VTABLE: LEMBALL 0x004953f8
class CEnemyGroupManager : public CGenericGroupManager {
public:
	CEnemyGroupManager(CAi* p_ai, CObjectManager* p_objectManager, CFormationManager* p_formationManager);
	LoadEnemyDataAdditionalAction* LoadLevelAdditionalWaypoint(LoadEnemyDataAdditionalAction* p_data,
															   WaypointInformation*& p_waypointInfo);
	void LoadLevel(LoadEnemyData* p_data, unsigned long p_dataSize, unsigned int p_skip);
	void Restart();
};

unsigned long EnemyGetLong(unsigned long* p_data);
#endif
