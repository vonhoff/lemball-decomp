#include "EnemyGroupManager.h"

// 68K 0x10608928 ENEMY_GetLONG__FPUl
// FUNCTION: LEMBALL 0x00420b50
unsigned long EnemyGetLong(unsigned long* p_data)
{
	return 0;
}

// 68K 0x10608950 __ct__18CEnemyGroupManagerFP3CAIP14CObjectManagerP17CFormationManager
// STUB: LEMBALL 0x00420b80
EnemyGroupManager::EnemyGroupManager(Ai* p_arg0, ObjectManager* p_arg1, FormationManager* p_arg2)
{
}

// 68K 0x10608a92 Restart__18CEnemyGroupManagerFv
// STUB: LEMBALL 0x00420bb0
void EnemyGroupManager::Restart()
{
}

// 68K 0x10608b06 LoadLevel__18CEnemyGroupManagerFP16tagLoadEnemyDataUlUc
// STUB: LEMBALL 0x00420dd0
void EnemyGroupManager::LoadLevel(LoadEnemyData* p_data, unsigned long p_dataSize, unsigned char p_skip)
{
}

// 68K 0x10608cba
// LoadLevelAdditional_Waypoint__18CEnemyGroupManagerFP32tagLoadEnemyDataAdditionalActionRP22tagWaypointInformation
// STUB: LEMBALL 0x00420f90
LoadEnemyDataAdditionalAction* EnemyGroupManager::LoadLevelAdditionalWaypoint(LoadEnemyDataAdditionalAction* p_data,
																			  WaypointInformation*& p_waypointInfo)
{
	return 0;
}
