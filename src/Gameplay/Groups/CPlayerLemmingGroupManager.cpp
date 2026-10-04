#include "CPlayerLemmingGroupManager.h"

enum {
	LEMMING_COUNTS_USE_DEFAULTS = -1
};

#include "Map/CMap.h"
#include "Map/CGround.h"
#include "Multiplayer/Transport/CConnect.h"
#include "Gameplay/Objects/CObjectManager.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Characters/CPlayerLemming.h"
#include "Gameplay/Objects/CViewData.h"
#include "CFormationManager.h"
#include "CPlayerLemmingGroup.h"

enum {
	DEFAULT_PLAYER_START_X_MAP_COORDINATE = 0x112,
	DEFAULT_PLAYER_START_Y_MAP_COORDINATE = 0x34a,
	PREALLOCATED_PLAYER_GROUP_COUNT = 5,
	PLAYER_CONTROL_SCAN_COUNT = 8,
	PLAYER_GROUP_FIRST_LEMMING_DELAY_MS = 3900,
	PLAYER_GROUP_LEMMING_SPAWN_INTERVAL_MS = 800,
	PLAYER_GROUP_TRAP_DOOR_FINAL_DELAY_MS = 4100
};

// GLOBAL: LEMBALL 0x0049d138
int g_anDefaultPlayerLemmingCounts[5][4] = {{0, 0, 0, 0}, {4, 0, 0, 0}, {3, 1, 0, 0}, {2, 1, 1, 0}, {1, 1, 1, 1}};

// FUNCTION: LEMBALL 0x00418400
CPlayerLemmingGroupManager::CPlayerLemmingGroupManager(CAI* p_ai,
													   CObjectManager* p_objectManager,
													   CFormationManager* p_formationManager)
	: CGenericGroupManager(p_ai, p_objectManager, p_formationManager),
	  CBaseObjectManager(NETWORK_OBJECT_MANAGER_MESSAGE_ID_BASE + OBJECT_MANAGER_TRANSPORT_PLAYER_LEMMING_GROUPS,
						 OBJECT_MANAGER_TRANSPORT_PLAYER_LEMMING_GROUPS)
{
	m_deadCount = 0;
	m_startX[0] = DEFAULT_PLAYER_START_X_MAP_COORDINATE;
	m_startZ[0] = 0;
	m_lemmingCounts[3] = 0;
	m_lemmingCounts[2] = 0;
	m_lemmingCounts[1] = 0;
	m_deleteEmptyGroups = false;
	m_networkInitialised = 0;
	m_startY[0] = DEFAULT_PLAYER_START_Y_MAP_COORDINATE;
	m_startPositionCount = 1;
	m_lemmingCounts[0] = 4;

	int remaining = PREALLOCATED_PLAYER_GROUP_COUNT;
	do {
		CPlayerLemmingGroup* group =
			new CPlayerLemmingGroup(g_pGenericGroupAI, g_pGenericGroupObjectManager, g_pGenericGroupFormationManager);
		group->Restart();
		AddNewGroup(group);
		remaining--;
	} while (remaining != 0);
}

// FUNCTION: LEMBALL 0x00418520
void CPlayerLemmingGroupManager::Restart()
{
	CGenericGroupManager::Restart();
	m_controlledGroupIndex = 0;
}

// FUNCTION: LEMBALL 0x00418540
CPlayerLemmingGroupManager::~CPlayerLemmingGroupManager()
{
	for (int i = 0; i < m_deadCount; i++) {
		delete m_dead[i];
	}
	if (m_networkInitialised != 0) {
		for (int i = 0; i < 4; i++) {
			delete m_networkLemmings[i];
		}
	}
}

// FUNCTION: LEMBALL 0x004185d0
CPlayerLemming* CPlayerLemmingGroupManager::GetDead()
{
	if (m_deadCount == 0) {
		return NULL;
	}
	m_deadCount--;
	return m_dead[m_deadCount];
}

// FUNCTION: LEMBALL 0x004185f0
bool CPlayerLemmingGroupManager::GetLeaderPos(AICOORD& p_position)
{
	CPlayerLemmingGroup* group = GetPlayerControlledGroup();
	if (group == NULL) {
		return false;
	}
	CGameObject* object = group->CGenericGroup::GetFirstElementInGroup();
	if (object == NULL) {
		return false;
	}
	p_position.m_xFixed = object->m_position.m_xFixed;
	p_position.m_yFixed = object->m_position.m_yFixed;
	p_position.m_zFixed = object->m_position.m_zFixed;
	return true;
}

// FUNCTION: LEMBALL 0x00418640
void CPlayerLemmingGroupManager::ProcessDead()
{
}

// FUNCTION: LEMBALL 0x00418650
void CPlayerLemmingGroupManager::Process()
{
	CPlayerLemming* lemming;
	CGenericGroupManager* manager = this;
	bool controlledGroupDeleted = false;
	CGenericGroup* genericGroup = manager->CGenericGroupManager::GetFirstGroup();
	while (genericGroup != NULL) {
		genericGroup->Process();
		genericGroup = manager->CGenericGroupManager::GetNextGroup();
	}

	CPlayerLemmingGroup* group = (CPlayerLemmingGroup*) manager->CGenericGroupManager::GetFirstGroup();
	while (group != NULL) {
		lemming = group->GetFirstDeadLemming();
		if (lemming != NULL) {
			do {
				group->RemoveLemmingFromGroup(lemming);
				m_dead[m_deadCount] = lemming;
				m_deadCount++;
				if (group->GetElementsInGroup() == 0) {
					lemming = NULL;
					if (GetPlayerControlledGroup() == group) {
						controlledGroupDeleted = true;
					}
					DeleteGroup(group);
				}
				else {
					lemming = group->GetFirstDeadLemming();
				}
			} while (lemming != NULL);
		}
		group = (CPlayerLemmingGroup*) manager->CGenericGroupManager::GetNextGroup();
	}
	if (controlledGroupDeleted) {
		MakePreviousGroupPlayerControlled();
	}
	ProcessDead();
}

// FUNCTION: LEMBALL 0x00418720
void CPlayerLemmingGroupManager::DeleteGroup(CPlayerLemmingGroup* p_group)
{
	p_group->Restart();
}

// FUNCTION: LEMBALL 0x00418730
void CPlayerLemmingGroupManager::CreateNewGroup(unsigned short p_count, unsigned short* p_objectIds)
{
	CPlayerLemmingGroup* group = NULL;
	CGenericGroup** groups;
	int index = 0;
	MakeNoGroupsPlayerControlled();
	if (m_groupCount > 0) {
		groups = m_groups;
		do {
			if ((*groups)->GetElementsInGroup() == 0) {
				group = (CPlayerLemmingGroup*) CGenericGroupManager::GetNthGroup(index);
				break;
			}
			groups++;
			index++;
		} while (index < m_groupCount);
	}

	int added = 0;
	if (p_count != 0) {
		unsigned int remaining = p_count;
		do {
			unsigned short objectId = *p_objectIds;
			p_objectIds++;
			CPlayerLemming* lemming = (CPlayerLemming*) g_pObjects[objectId];
			if (lemming->IsSelectable()) {
				lemming->ResetInstructions();
				AddPlayerLemmingToGroup(lemming, group);
				lemming->EmptyDestinationList();
				added++;
			}
			remaining--;
		} while (remaining != 0);
	}

	if (added > 0) {
		MakeParticularGroupPlayerControlled(group);
		ReformAlteredGroups(group);
	}
}

// FUNCTION: LEMBALL 0x004187f0
bool CPlayerLemmingGroupManager::RemovePlayerLemmingFromGroup(CGameObject* p_object, CGenericGroup* p_group)
{
	bool removed = CGenericGroupManager::RemoveElementFromGroup(p_object, p_group);
	if (!removed) {
		MakePreviousGroupPlayerControlled();
	}
	return removed;
}

// FUNCTION: LEMBALL 0x00418820
void CPlayerLemmingGroupManager::AddPlayerLemmingToGroup(CPlayerLemming* p_lemming, CPlayerLemmingGroup* p_group)
{
	CGenericGroupManager::FindElementInGroupAndRemoveIt(p_lemming);
	p_group->AddLemmingToGroup(p_lemming);
}

// FUNCTION: LEMBALL 0x00418840
bool CPlayerLemmingGroupManager::IsLemmingPlayerControlled(CPlayerLemming* p_lemming)
{
	CPlayerLemmingGroup* group =
		(CPlayerLemmingGroup*) CGenericGroupManager::GetGroupElementIsMemberOf((CGameObject*) p_lemming);
	if (group != NULL) {
		return group->CheckPlayerControlled();
	}
	return false;
}

// FUNCTION: LEMBALL 0x00418860
bool CPlayerLemmingGroupManager::MakeNextGroupPlayerControlled()
{
	MakeNoGroupsPlayerControlled();
	int checked = 0;
	if (m_groupCount > 0) {
		do {
			m_controlledGroupIndex++;
			if (m_controlledGroupIndex == m_groupCount) {
				m_controlledGroupIndex = 0;
			}
			CPlayerLemmingGroup* group = (CPlayerLemmingGroup*) m_groups[m_controlledGroupIndex];
			if (group != NULL && group->GetElementsInGroup() > 0) {
				((CPlayerLemmingGroup*) m_groups[m_controlledGroupIndex])->SetPlayerControlled(1, NULL);
				return true;
			}
			checked++;
		} while (checked < m_groupCount);
	}
	return false;
}

// FUNCTION: LEMBALL 0x004188e0
bool CPlayerLemmingGroupManager::MakePreviousGroupPlayerControlled()
{
	MakeNoGroupsPlayerControlled();
	MakeNoGroupsPlayerControlled();
	int checked = 0;
	if (m_groupCount > 0) {
		do {
			m_controlledGroupIndex--;
			if (m_controlledGroupIndex < 0) {
				m_controlledGroupIndex += m_groupCount;
			}
			CPlayerLemmingGroup* group = (CPlayerLemmingGroup*) m_groups[m_controlledGroupIndex];
			if (group != NULL && group->GetElementsInGroup() > 0) {
				((CPlayerLemmingGroup*) m_groups[m_controlledGroupIndex])->SetPlayerControlled(1, NULL);
				return true;
			}
			checked++;
		} while (checked < m_groupCount);
	}
	return false;
}

// FUNCTION: LEMBALL 0x00418960
bool CPlayerLemmingGroupManager::MakeParticularGroupPlayerControlled(CPlayerLemmingGroup* p_group)
{
	MakeNoGroupsPlayerControlled();
	int index = 0;
	CGenericGroup** groups = m_groups;
	do {
		if (*groups == p_group) {
			m_controlledGroupIndex = index;
			if (p_group->GetElementsInGroup() == 0) {
				return MakeNextGroupPlayerControlled();
			}
			p_group->SetPlayerControlled(1, NULL);
			return true;
		}
		groups++;
		index++;
	} while (index < PLAYER_CONTROL_SCAN_COUNT);
	return false;
}

// FUNCTION: LEMBALL 0x004189c0
bool CPlayerLemmingGroupManager::MakeNoGroupsPlayerControlled()
{
	for (int i = 0; i < PLAYER_CONTROL_SCAN_COUNT; i++) {
		if (m_groups[i] != NULL) {
			((CPlayerLemmingGroup*) m_groups[i])->SetPlayerControlled(0, NULL);
		}
	}
	return true;
}

// FUNCTION: LEMBALL 0x004189f0
CPlayerLemmingGroup* CPlayerLemmingGroupManager::GetPlayerControlledGroup()
{
	int i = 0;
	if (m_groupCount > 0) {
		CGenericGroup** groups = m_groups;
		do {
			CPlayerLemmingGroup* group = (CPlayerLemmingGroup*) *groups;
			if (group != NULL && group->CheckPlayerControlled() == true) {
				return (CPlayerLemmingGroup*) m_groups[i];
			}
			groups++;
			i++;
		} while (m_groupCount > i);
	}
	return NULL;
}

// FUNCTION: LEMBALL 0x00418a30
void CPlayerLemmingGroupManager::AddNewWaypointToCurrentGroup(int p_x, int p_y)
{
	AICOORD coordinate(p_x << FIXED_POINT_FRACTION_BITS, p_y << FIXED_POINT_FRACTION_BITS, 0);
	CPlayerLemmingGroup* group = GetPlayerControlledGroup();
	if (group != NULL) {
		group->AddNewWaypoint(coordinate, g_pGenericGroupFormationManager);
	}
}

// FUNCTION: LEMBALL 0x00418a90
void CPlayerLemmingGroupManager::RemoveWaypointsFromCurrentGroup()
{
	CPlayerLemmingGroup* group = GetPlayerControlledGroup();
	if (group != NULL) {
		group->ClearExistingWaypoints();
	}
}

// FUNCTION: LEMBALL 0x00418ab0
void CPlayerLemmingGroupManager::UseObject(int p_objectId)
{
	CPlayerLemmingGroup* controlledGroup = GetPlayerControlledGroup();
	if (controlledGroup == NULL) {
		return;
	}
	CGameObject* object = g_pObjects[(unsigned short) p_objectId];
	if (object->m_objectType != OBJECT_PLAYER_2) {
		controlledGroup->AddUseObject(p_objectId);
		return;
	}
	CPlayerLemming* lemming = (CPlayerLemming*) object;
	if (lemming->m_action != ACTION_DEAD) {
		CPlayerLemmingGroup* group = lemming->GetGroup();
		if (group != controlledGroup) {
			controlledGroup->SetPlayerControlled(0, NULL);
		}
		group->SetPlayerControlled(1, lemming);
	}
}

// FUNCTION: LEMBALL 0x00418b20
void CPlayerLemmingGroupManager::ReformAlteredGroups(CPlayerLemmingGroup* p_excludedGroup)
{
	CPlayerLemmingGroup* group = (CPlayerLemmingGroup*) CGenericGroupManager::GetFirstGroup();
	while (group != NULL) {
		if (group != p_excludedGroup) {
			group->ReformAlteredGroup(g_pGenericGroupFormationManager);
		}
		group = (CPlayerLemmingGroup*) CGenericGroupManager::GetNextGroup();
	}
}

// FUNCTION: LEMBALL 0x00418b60
void CPlayerLemmingGroupManager::PlayerGroupRequestFire(int p_x, int p_y)
{
	CPlayerLemmingGroup* group = GetPlayerControlledGroup();
	if (group != NULL) {
		CPlayerLemming* lemming = (CPlayerLemming*) group->CGenericGroup::GetFirstElementInGroup();
		while (lemming != NULL) {
			lemming->RequestFire(p_x, p_y);
			lemming = (CPlayerLemming*) group->CGenericGroup::GetNextElementInGroup();
		}
	}
}

// FUNCTION: LEMBALL 0x00418ba0
void CPlayerLemmingGroupManager::GetPlayerStartPosition(AICOORD& p_position, int p_index)
{
	p_position.m_xFixed = m_startX[p_index] << FIXED_POINT_FRACTION_BITS;
	p_position.m_yFixed = m_startY[p_index] << FIXED_POINT_FRACTION_BITS;
	p_position.m_zFixed = m_startZ[p_index] << FIXED_POINT_FRACTION_BITS;
}

// FUNCTION: LEMBALL 0x00418be0
void CPlayerLemmingGroupManager::SetLemmingCounts(int p_playerCount,
												  int p_count0,
												  int p_count1,
												  int p_count2,
												  int p_count3)
{
	m_startPositionCount = p_playerCount;
	if (p_count0 == LEMMING_COUNTS_USE_DEFAULTS) {
		m_lemmingCounts[0] = g_anDefaultPlayerLemmingCounts[p_playerCount][0];
		m_lemmingCounts[1] = g_anDefaultPlayerLemmingCounts[p_playerCount][1];
		m_lemmingCounts[2] = g_anDefaultPlayerLemmingCounts[p_playerCount][2];
		m_lemmingCounts[3] = g_anDefaultPlayerLemmingCounts[p_playerCount][3];
	}
	else {
		m_lemmingCounts[0] = p_count0;
		m_lemmingCounts[1] = p_count1;
		m_lemmingCounts[2] = p_count2;
		m_lemmingCounts[3] = p_count3;
	}
	for (int i = 0; i < p_playerCount; i++) {
		if (m_startX[i] > MAP_COORDINATE_MAX || m_startX[i] < 0) {
			m_startX[i] = i * GROUND_BLOCK_PIXEL_SIZE;
		}
		if (m_startY[i] > MAP_COORDINATE_MAX || m_startY[i] < 0) {
			m_startY[i] = i * GROUND_BLOCK_PIXEL_SIZE;
		}
	}
}

// FUNCTION: LEMBALL 0x00418c90
int CPlayerLemmingGroupManager::GetLemmingCountForPlayer(int p_playerIndex)
{
	return m_lemmingCounts[p_playerIndex];
}

// FUNCTION: LEMBALL 0x00418ca0
void CPlayerLemmingGroupManager::InitialiseNetwork()
{
	if (g_pActiveConnection != NULL) {
		CPlayerLemming** lemmings = m_networkLemmings;
		int remaining = 4;
		do {
			if (m_networkInitialised == 0) {
				*lemmings = new CPlayerLemming(0, 0, 0, 0, 1, 0);
			}
			(*lemmings)->Restart();
			CBaseObjectManager* manager = this;
			(*lemmings)->m_manager = manager;
			lemmings++;
			remaining--;
		} while (remaining != 0);
		m_networkInitialised = 1;
	}
}

// FUNCTION: LEMBALL 0x00418d20
void CPlayerLemmingGroupManager::LoadLevel(unsigned char* p_data, unsigned long p_dataSize, unsigned int p_skip)
{
	unsigned short* data = (unsigned short*) p_data;
	m_startPositionCount = 1;
	int x = *data++;
	m_startX[0] = x;
	int y = *data++;
	m_startY[0] = y;
	m_startZ[0] = *data++;
	CMap* map = g_pMap;
	int blockY;
	int blockX;
	blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
	blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
	unsigned short z;
	if (x >= 0 && y >= 0) {
		int width = map->m_ground.m_width;
		if (blockX < width && map->m_ground.m_height > blockY) {
			x &= GROUND_BLOCK_PIXEL_MASK;
			y &= GROUND_BLOCK_PIXEL_MASK;
			z = map->m_ground.m_ground[blockY * width + blockX].GetZ(x, y);
		}
		else {
			z = 0;
		}
	}
	else {
		z = 0;
	}
	m_startZ[0] = z;
	int remaining;
	int start = 1;
	remaining = 3;
	do {
		int current = m_startX[0];
		m_startX[start] = current;
		m_startY[start] = m_startY[0];
		m_startZ[start] = m_startZ[0];
		start++;
	} while (--remaining != 0);
	CPlayerLemming** reuse = NULL;
	int count = g_pGenericGroupAI->m_lemmingCount;
	int dead = 4 - count;
	if (p_skip != 0) {
		reuse = g_pGenericGroupAI->m_networkLemmings;
	}
	m_deadCount = 0;
	int i;
	for (i = 0; i < dead; i++) {
		CPlayerLemming* lemming;
		if (reuse == NULL) {
			lemming = new CPlayerLemming(m_startX[i], m_startY[i], m_startZ[i], 0, 0, 0);
		}
		else {
			lemming = *reuse++;
		}
		lemming->Restart();
		CBaseObjectManager* manager = this;
		lemming->m_manager = manager;
		lemming->m_action = ACTION_DEAD;
		int& objectCount = g_pGenericGroupAI->m_objectCount;
		for (int j = 0; j < objectCount; j++) {
			CGameObject**& objects = g_pGenericGroupAI->m_objects;
			if (objects[j] == lemming) {
				objectCount--;
				for (; j < objectCount; j++) {
					objects[j] = objects[j + 1];
				}
				objects[objectCount] = NULL;
				break;
			}
		}
		g_wLemmingCount--;
		m_dead[m_deadCount++] = lemming;
	}
	CPlayerLemmingGroup* group = (CPlayerLemmingGroup*) CGenericGroupManager::GetFirstGroup();
	for (i = 0; i < count; i++) {
		int delay =
			(PLAYER_GROUP_FIRST_LEMMING_DELAY_MS + i * PLAYER_GROUP_LEMMING_SPAWN_INTERVAL_MS) / GAME_TICK_MILLISECONDS;
		CPlayerLemming* lemming;
		if (reuse == NULL) {
			lemming = new CPlayerLemming(m_startX[i], m_startY[i], m_startZ[i], 0, 0, delay);
		}
		else {
			lemming = *reuse++;
		}
		lemming->Restart();
		CBaseObjectManager* manager = this;
		lemming->m_manager = manager;
		AddPlayerLemmingToGroup(lemming, group);
	}
	unsigned int doorTime =
		(count * PLAYER_GROUP_LEMMING_SPAWN_INTERVAL_MS + PLAYER_GROUP_FIRST_LEMMING_DELAY_MS) / GAME_TICK_MILLISECONDS;
	if (g_pGenericGroupAI->m_gameplayStartDelay < doorTime) {
		g_pGenericGroupAI->m_gameplayStartDelay = doorTime;
	}
	m_lemmingCounts[0] = count;
	m_lemmingCounts[3] = 0;
	m_lemmingCounts[2] = 0;
	m_lemmingCounts[1] = 0;
	MakeParticularGroupPlayerControlled(group);
	if (p_skip == 0) {
		g_pGenericGroupAI->AddNewTrapDoor(m_startX[0], m_startY[0], m_startZ[0], doorTime);
	}
}

// FUNCTION: LEMBALL 0x00419060
void CPlayerLemmingGroupManager::LoadAdditionalPlayerStartPositions(unsigned char* p_data,
																	unsigned long p_dataSize,
																	unsigned int p_skip)
{
	CMap* map = g_pMap;
	unsigned short* data = (unsigned short*) p_data;
	m_startPositionCount = *data++;
	int total = 0;
	int i;
	for (i = 0; i < m_startPositionCount; i++) {
		m_startX[i] = *data++;
		m_startY[i] = *data++;
		m_startZ[i] = *data++;
		int y;
		int x;
		x = m_startX[i];
		y = m_startY[i];
		int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
		int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
		unsigned short z;
		if (x >= 0 && y >= 0) {
			int width = map->m_ground.m_width;
			if (blockX < width && map->m_ground.m_height > blockY) {
				z = map->m_ground.m_ground[blockY * width + blockX].GetZ(x & GROUND_BLOCK_PIXEL_MASK,
																		 y & GROUND_BLOCK_PIXEL_MASK);
			}
			else {
				z = 0;
			}
		}
		else {
			z = 0;
		}
		m_startZ[i] = z;
		m_lemmingCounts[i] = *data++;
		total += m_lemmingCounts[i];
	}
	g_pGenericGroupAI->NLemmings(total);
	int dead = 4 - total;
	CPlayerLemming** reuse = NULL;
	if (p_skip != 0) {
		reuse = g_pGenericGroupAI->m_networkLemmings;
	}
	m_deadCount = 0;
	for (i = 0; i < dead; i++) {
		CPlayerLemming* lemming;
		if (reuse == NULL) {
			lemming = new CPlayerLemming(m_startX[i], m_startY[i], m_startZ[i], 0, 0, 0);
		}
		else {
			lemming = *reuse++;
		}
		lemming->Restart();
		lemming->m_action = ACTION_DEAD;
		int& objectCount = g_pGenericGroupAI->m_objectCount;
		for (int j = 0; j < objectCount; j++) {
			CGameObject**& objects = g_pGenericGroupAI->m_objects;
			if (objects[j] == lemming) {
				objectCount--;
				for (; j < objectCount; j++) {
					objects[j] = objects[j + 1];
				}
				objects[objectCount] = NULL;
				break;
			}
		}
		g_wLemmingCount--;
		CBaseObjectManager* manager = this;
		lemming->m_manager = manager;
		m_dead[m_deadCount++] = lemming;
	}
	for (i = 0; i < m_startPositionCount; i++) {
		for (int j = 0; j < m_lemmingCounts[i]; j++) {
			int delay = (PLAYER_GROUP_FIRST_LEMMING_DELAY_MS + j * PLAYER_GROUP_LEMMING_SPAWN_INTERVAL_MS) /
						GAME_TICK_MILLISECONDS;
			CPlayerLemming* lemming;
			if (reuse == NULL) {
				lemming = new CPlayerLemming(m_startX[i], m_startY[i], m_startZ[i], 0, 0, delay);
			}
			else {
				lemming = *reuse++;
			}
			lemming->Restart();
			CBaseObjectManager* manager = this;
			lemming->m_manager = manager;
			CPlayerLemmingGroup* group = (CPlayerLemmingGroup*) CGenericGroupManager::GetNthGroup(i);
			AddPlayerLemmingToGroup(lemming, group);
		}
		unsigned int doorTime =
			(m_lemmingCounts[i] * PLAYER_GROUP_LEMMING_SPAWN_INTERVAL_MS + PLAYER_GROUP_FIRST_LEMMING_DELAY_MS) /
			GAME_TICK_MILLISECONDS;
		if (g_pGenericGroupAI->m_gameplayStartDelay < doorTime) {
			g_pGenericGroupAI->m_gameplayStartDelay = doorTime;
		}
		doorTime =
			(m_lemmingCounts[i] * PLAYER_GROUP_LEMMING_SPAWN_INTERVAL_MS + PLAYER_GROUP_TRAP_DOOR_FINAL_DELAY_MS) /
			GAME_TICK_MILLISECONDS;
		if (p_skip == 0) {
			g_pGenericGroupAI->AddNewTrapDoor(m_startX[i], m_startY[i], m_startZ[i], doorTime);
		}
	}
	MakeParticularGroupPlayerControlled((CPlayerLemmingGroup*) CGenericGroupManager::GetFirstGroup());
}

// FUNCTION: LEMBALL 0x004193f0
bool CPlayerLemmingGroupManager::CheckNetworkStateChanged()
{
	bool changed = false;
	for (int index = 0; index < m_groupCount; ++index) {
		CPlayerLemmingGroup* group = (CPlayerLemmingGroup*) m_groups[index];
		bool groupChanged = group->CheckNetworkStateChanged();
		changed = groupChanged || changed;
	}
	return changed;
}

// FUNCTION: LEMBALL 0x00419440
bool CPlayerLemmingGroupManager::HasSFXChanged()
{
	int changed = 0;
	int i = 0;
	if (m_groupCount > 0) {
		CGenericGroup** group = m_groups;
		do {
			if (((CPlayerLemmingGroup*) *group)->HasSFXChanged() != 0 || changed != 0) {
				changed = 1;
			}
			else {
				changed = 0;
			}
			group++;
			i++;
		} while (m_groupCount > i);
	}
	return changed;
}

// FUNCTION: LEMBALL 0x00419490
int CPlayerLemmingGroupManager::GetViewData(CViewData* p_viewData)
{
	int count = 0;
	if (g_pActiveConnection != NULL) {
		CPlayerLemming** lemmingCursor = m_networkLemmings;
		int remaining = 4;
		CViewData* viewCursor = p_viewData;
		count = 4;
		do {
			CPlayerLemming* lemming = *lemmingCursor++;
			lemming->GetViewData(*viewCursor++);
		} while (--remaining != 0);
		p_viewData = viewCursor;
	}
	return CGenericGroupManager::GetViewData(p_viewData) + count;
}
