#include "CEnemyGroupManager.h"

#include "../Base/tagLoadEnemyData.h"
#include "../Base/tagWaypointInformation.h"
#include "../Navigation/CAI.h"
#include "../Objects/CEnemy.h"
#include "AI/Base/CGameObject.h"
#include "AI/Groups/CGenericGroup.h"
#include "AI/Groups/CGenericGroupManager.h"
#include "CEnemyGroup.h"

#define ENEMY_LEVEL_HEADER_BYTES (2 * sizeof(unsigned short))
#define ENEMY_WAYPOINT_DESCRIPTOR_BYTES 4
#define ENEMY_WAYPOINT_STEP_SIGN_BIT 0x80
#define ENEMY_WAYPOINT_STEP_SIGN_EXTENSION_MASK 0xffffff00

extern CAI* g_pGenericGroupAI;
extern CObjectManager* g_pGenericGroupObjectManager;
extern CFormationManager* g_pGenericGroupFormationManager;

// FUNCTION: LEMBALL 0x00420b50
unsigned long ENEMY_GetLONG(unsigned long* p_data)
{
	unsigned char* data;

	data = (unsigned char*) p_data;
	return (((unsigned long) data[3] << 16 | (unsigned long) data[1]) << 8) | (unsigned long) data[2] << 16 |
		   (unsigned long) data[0];
}

// FUNCTION: LEMBALL 0x00420b80
CEnemyGroupManager::CEnemyGroupManager(CAI* p_ai,
									   CObjectManager* p_objectManager,
									   CFormationManager* p_formationManager)
	: CGenericGroupManager(p_ai, p_objectManager, p_formationManager)
{
}

// FUNCTION: LEMBALL 0x00420bb0
void CEnemyGroupManager::Restart()
{
	CGenericGroup** group;
	int groupIndex = 0;
	if (groupIndex < m_groupCount) {
		group = m_groups;
		do {
			int elementCount = (*group)->GetElementsInGroup();
			int elementIndex = 0;
			if (elementCount > 0) {
				do {
					(*group)->GetNthElementInGroup(elementIndex)->Restart();
					elementIndex++;
				} while (elementIndex < elementCount);
			}
			group++;
			groupIndex++;
		} while (groupIndex < m_groupCount);
	}
}

// FUNCTION: LEMBALL 0x00420c10
void CEnemyGroupManager::AddEnemyGroup(int p_x,
									   int p_y,
									   int p_z,
									   eEnemyStateActions p_action0,
									   eEnemyStateRules p_rule0,
									   eEnemyStateActions p_action1,
									   eEnemyStateRules p_rule1,
									   eEnemyStateActions p_action2,
									   eEnemyStateRules p_rule2,
									   unsigned short p_waypointStart,
									   int p_waypointCount)
{
	int i;
	CEnemyGroup* group =
		new CEnemyGroup(g_pGenericGroupAI, g_pGenericGroupObjectManager, g_pGenericGroupFormationManager);
	CGenericGroupManager::AddNewGroup(group);
	group->SetFormationIndex(1);
	CEnemy* enemy = new CEnemy(g_pGenericGroupAI, p_x, p_y, p_z, 0);
	enemy->Restart();
	enemy->SetEnemyType(p_action0, p_rule0, p_action1, p_rule1, p_action2, p_rule2);
	CGenericGroupManager::AddElementToGroup(enemy, group);
	if (p_action0 == ENEMY_ACTION_PATROL) {
		tagWaypointInformation* waypoint = new tagWaypointInformation;
		waypoint->m_patrolMode = WAYPOINT_PATROL_LOOP;
		waypoint->m_waypointCount = p_waypointCount;
		waypoint->m_waypointIndex = 0;
		waypoint->m_waypointStep = 1;
		waypoint->m_waypoints = new unsigned short[p_waypointCount];
		for (i = 0; i < p_waypointCount; i++) {
			waypoint->m_waypoints[i] = (unsigned short) (p_waypointStart + i);
		}
		enemy->m_state0Data.m_waypointInformation = waypoint;
	}
}

// FUNCTION: LEMBALL 0x00420d30
void CEnemyGroupManager::RemoveEnemyGroup(CEnemyGroup* p_group)
{
	int i = 0;
	int& count = g_pGenericGroupAI->m_objectCount;
	int originalCount = count;
	if (i < originalCount) {
		CGameObject**& objects = g_pGenericGroupAI->m_objects;
		do {
			if (objects[i] == p_group) {
				count--;
				while (i < count) {
					objects[i] = objects[i + 1];
					i++;
				}
				objects[count] = 0;
				break;
			}
			i++;
		} while (i < originalCount);
	}
	FindElementInGroupAndRemoveIt(p_group);
	p_group->Delete();
	delete p_group;
}

// FUNCTION: LEMBALL 0x00420dd0
void CEnemyGroupManager::LoadLevel(tagLoadEnemyData* p_data, unsigned long p_dataSize, unsigned int p_skip)
{
	unsigned short* wordData = (unsigned short*) p_data;
	int headerCount = *wordData;
	int count;
	unsigned char* data = (unsigned char*) p_data + ENEMY_LEVEL_HEADER_BYTES;
	unsigned int x;
	unsigned int y;
	unsigned int facing;
	eEnemyStateActions action1;
	eEnemyStateActions action0;
	eEnemyStateRules rule0;
	eEnemyStateRules rule1;
	eEnemyStateActions action2;
	eEnemyStateRules rule2;
	tagWaypointInformation* waypoint0;
	tagWaypointInformation* waypoint1;
	tagWaypointInformation* waypoint2;

	if (p_skip != 0) {
		return;
	}
	if (headerCount <= 0) {
		return;
	}
	count = headerCount;

	do {
		tagLoadEnemyData* enemyData = (tagLoadEnemyData*) data;
		x = enemyData->m_x;
		y = enemyData->m_y;
		facing = enemyData->m_facing;
		action0 = (eEnemyStateActions) enemyData->m_action0;
		rule0 = (eEnemyStateRules) enemyData->m_rule0;
		action1 = (eEnemyStateActions) enemyData->m_action1;
		rule1 = (eEnemyStateRules) enemyData->m_rule1;
		action2 = (eEnemyStateActions) enemyData->m_action2;
		rule2 = (eEnemyStateRules) enemyData->m_rule2;
		data += sizeof(*enemyData);

		CEnemyGroup* group =
			new CEnemyGroup(g_pGenericGroupAI, g_pGenericGroupObjectManager, g_pGenericGroupFormationManager);
		group->Restart();
		CGenericGroupManager::AddNewGroup(group);
		group->SetFormationIndex(1);

		CEnemy* enemy = new CEnemy(g_pGenericGroupAI, x, y, 0, facing);
		enemy->Restart();
		enemy->SetEnemyType(action0, rule0, action1, rule1, action2, rule2);

		if (action0 == ENEMY_ACTION_PATROL) {
			data = (unsigned char*) LoadLevelAdditional_Waypoint((tagLoadEnemyDataAdditionalAction*) data, waypoint0);
			enemy->m_state0Data.m_waypointInformation = waypoint0;
		}
		if (action1 == ENEMY_ACTION_PATROL) {
			data = (unsigned char*) LoadLevelAdditional_Waypoint((tagLoadEnemyDataAdditionalAction*) data, waypoint1);
			enemy->m_state1Data.m_waypointInformation = waypoint1;
		}
		if (action2 == ENEMY_ACTION_PATROL) {
			data = (unsigned char*) LoadLevelAdditional_Waypoint((tagLoadEnemyDataAdditionalAction*) data, waypoint2);
			enemy->m_state2Data.m_waypointInformation = waypoint2;
		}

		CGenericGroupManager::AddElementToGroup(enemy, group);
		count--;
	} while (count != 0);
}

// FUNCTION: LEMBALL 0x00420f90
tagLoadEnemyDataAdditionalAction* CEnemyGroupManager::LoadLevelAdditional_Waypoint(
	tagLoadEnemyDataAdditionalAction* p_data,
	tagWaypointInformation*& p_waypointInfo)
{
	unsigned char* data = (unsigned char*) p_data;
	ENEMY_GetLONG((unsigned long*) data);
	data += sizeof(unsigned long);

	p_waypointInfo = new tagWaypointInformation;
	unsigned int waypointCount = data[1];
	p_waypointInfo->m_patrolMode = data[0];
	p_waypointInfo->m_waypointCount = waypointCount;
	p_waypointInfo->m_waypointIndex = data[2];

	unsigned char rawWaypointStep = data[3];
	int waypointStep;
	if ((rawWaypointStep & ENEMY_WAYPOINT_STEP_SIGN_BIT) != 0) {
		waypointStep = rawWaypointStep | ENEMY_WAYPOINT_STEP_SIGN_EXTENSION_MASK;
	}
	else {
		waypointStep = rawWaypointStep;
	}
	p_waypointInfo->m_waypointStep = waypointStep;

	p_waypointInfo->m_waypoints = new unsigned short[waypointCount];
	if ((int) waypointCount > 0) {
		unsigned short* waypointData = (unsigned short*) (data + ENEMY_WAYPOINT_DESCRIPTOR_BYTES);
		int i = 0;
		unsigned int remaining = waypointCount;
		do {
			p_waypointInfo->m_waypoints[i] = *waypointData++;
			i++;
			remaining--;
		} while (remaining != 0);
	}

	return (tagLoadEnemyDataAdditionalAction*) (data + waypointCount * sizeof(*p_waypointInfo->m_waypoints) +
												ENEMY_WAYPOINT_DESCRIPTOR_BYTES);
}
