#include "EnemyGroupManager.h"

#include "../Base/WaypointInformation.h"
#include "../Objects/Enemy.h"
#include "EnemyGroup.h"

extern Ai* g_pGenericGroupAI;
extern ObjectManager* g_pGenericGroupObjectManager;
extern FormationManager* g_pGenericGroupFormationManager;

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
// FUNCTION: LEMBALL 0x00420b80
EnemyGroupManager::EnemyGroupManager(Ai* p_arg0, ObjectManager* p_arg1, FormationManager* p_arg2)
	: GenericGroupManager(p_arg0, p_arg1, p_arg2)
{
}

// 68K 0x10608a92 Restart__18CEnemyGroupManagerFv
// STUB: LEMBALL 0x00420bb0
void EnemyGroupManager::Restart()
{
}

// 68K 0x10608b06 LoadLevel__18CEnemyGroupManagerFP16tagLoadEnemyDataUlUc
// FUNCTION: LEMBALL 0x00420dd0
void EnemyGroupManager::LoadLevel(LoadEnemyData* p_data, unsigned long p_dataSize, unsigned int p_skip)
{
	unsigned short* wordData = (unsigned short*) p_data;
	int headerCount = *wordData;
	int count;
	wordData += 2;
	unsigned char* data = (unsigned char*) wordData;
	unsigned int x;
	unsigned int y;
	unsigned int facing;
	register eEnemyStateActions action1;
	eEnemyStateActions action0;
	eEnemyStateRules rule0;
	eEnemyStateRules rule1;
	eEnemyStateActions action2;
	eEnemyStateRules rule2;
	WaypointInformation* waypoint0;
	WaypointInformation* waypoint1;
	WaypointInformation* waypoint2;

	if (p_skip != 0) {
		return;
	}
	if (headerCount <= 0) {
		return;
	}
	count = headerCount;

	do {
		wordData = (unsigned short*) data;
		x = wordData[0];
		y = wordData[1];
		facing = data[4];
		action0 = (eEnemyStateActions) data[5];
		rule0 = (eEnemyStateRules) data[6];
		action1 = (eEnemyStateActions) data[7];
		rule1 = (eEnemyStateRules) data[8];
		action2 = (eEnemyStateActions) data[9];
		rule2 = (eEnemyStateRules) data[10];
		data += 12;

		EnemyGroup* group =
			new EnemyGroup(g_pGenericGroupAI, g_pGenericGroupObjectManager, g_pGenericGroupFormationManager);
		group->Restart();
		GenericGroupManager::AddNewGroup(group);
		group->SetFormationIndex(1);

		Enemy* enemy = new Enemy(g_pGenericGroupAI, x, y, 0, facing);
		enemy->Restart();
		enemy->SetEnemyType(action0, rule0, action1, rule1, action2, rule2);

		if (action0 == 1) {
			data = (unsigned char*) LoadLevelAdditionalWaypoint((LoadEnemyDataAdditionalAction*) data, waypoint0);
			enemy->m_state0Data.m_waypointInformation = waypoint0;
		}
		if (action1 == 1) {
			data = (unsigned char*) LoadLevelAdditionalWaypoint((LoadEnemyDataAdditionalAction*) data, waypoint1);
			enemy->m_state1Data.m_waypointInformation = waypoint1;
		}
		if (action2 == 1) {
			data = (unsigned char*) LoadLevelAdditionalWaypoint((LoadEnemyDataAdditionalAction*) data, waypoint2);
			enemy->m_state2Data.m_waypointInformation = waypoint2;
		}

		GenericGroupManager::AddElementToGroup(enemy, group);
		count--;
	} while (count != 0);
}

// clang-format off
// 68K 0x10608cba LoadLevelAdditional_Waypoint__18CEnemyGroupManagerFP32tagLoadEnemyDataAdditionalActionRP22tagWaypointInformation
// clang-format on
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
