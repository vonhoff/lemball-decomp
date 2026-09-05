#include "EnemyGroupManager.h"

#include "../Base/WaypointInformation.h"

// 68K 0x10608928 ENEMY_GetLONG__FPUl
// FUNCTION: LEMBALL 0x00420b50
unsigned long EnemyGetLong(unsigned long* p_data)
{
	unsigned char* data;

	data = (unsigned char*) p_data;
	return (((unsigned long) data[3] << 16 | (unsigned long) data[1]) << 8) | (unsigned long) data[2] << 16 |
		   (unsigned long) data[0];
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
// FUNCTION: LEMBALL 0x00420f90
LoadEnemyDataAdditionalAction* EnemyGroupManager::LoadLevelAdditionalWaypoint(LoadEnemyDataAdditionalAction* p_data,
																			  WaypointInformation*& p_waypointInfo)
{
	unsigned char* data = (unsigned char*) p_data;
	EnemyGetLong((unsigned long*) data);
	data += 4;

	p_waypointInfo = new WaypointInformation;
	unsigned int waypointCount = data[1];
	p_waypointInfo->m_action = data[0];
	p_waypointInfo->m_waypointCount = waypointCount;
	p_waypointInfo->m_value = data[2];

	unsigned char rawSignedValue = data[3];
	int signedValue;
	if ((rawSignedValue & 0x80) != 0) {
		signedValue = rawSignedValue | 0xffffff00;
	}
	else {
		signedValue = rawSignedValue;
	}
	p_waypointInfo->m_signedValue = signedValue;

	p_waypointInfo->m_waypoints = new unsigned short[waypointCount];
	if ((int) waypointCount > 0) {
		unsigned short* waypointData = (unsigned short*) data;
		waypointData += 2;
		int i = 0;
		unsigned int remaining = waypointCount;
		do {
			p_waypointInfo->m_waypoints[i] = *waypointData++;
			i++;
			remaining--;
		} while (remaining != 0);
	}

	return (LoadEnemyDataAdditionalAction*) (data + waypointCount * 2 + 4);
}
