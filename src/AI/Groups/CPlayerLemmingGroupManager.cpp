#include "CPlayerLemmingGroupManager.h"

#include "../../Map/Base/CMap.h"
#include "../../Visos/Network/CConnect.h"
#include "../Managers/CObjectManager.h"
#include "../Navigation/CAi.h"
#include "../Objects/CPlayerLemming.h"
#include "../Objects/CViewData.h"
#include "CFormationManager.h"
#include "CPlayerLemmingGroup.h"

// GLOBAL: LEMBALL 0x0049d138
int g_anDefaultPlayerLemmingCounts[5][4] = {{0, 0, 0, 0}, {4, 0, 0, 0}, {3, 1, 0, 0}, {2, 1, 1, 0}, {1, 1, 1, 1}};

// FUNCTION: LEMBALL 0x00418400
CPlayerLemmingGroupManager::CPlayerLemmingGroupManager(CAi* p_arg0, CObjectManager* p_arg1, CFormationManager* p_arg2)
	: CGenericGroupManager(p_arg0, p_arg1, p_arg2), CBaseObjectManager(0x22, 0x17)
{
	m_deadCount = 0;
	m_startX[0] = 0x112;
	m_startZ[0] = 0;
	m_lemmingCounts[3] = 0;
	m_lemmingCounts[2] = 0;
	m_lemmingCounts[1] = 0;
	m_state = 0;
	m_networkInitialized = 0;
	m_startY[0] = 0x34a;
	m_startPositionCount = 1;
	m_lemmingCounts[0] = 4;

	int remaining = 5;
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
	if (m_networkInitialized != 0) {
		for (int i = 0; i < 4; i++) {
			delete m_networkLemmings[i];
		}
	}
}

// FUNCTION: LEMBALL 0x004185d0
CPlayerLemming* CPlayerLemmingGroupManager::GetDead()
{
	if (m_deadCount == 0) {
		return 0;
	}
	m_deadCount--;
	return m_dead[m_deadCount];
}

// FUNCTION: LEMBALL 0x004185f0
bool CPlayerLemmingGroupManager::GetLeaderPos(AiCoord& p_position)
{
	CPlayerLemmingGroup* group = GetPlayerControlledGroup();
	if (group == 0) {
		return 0;
	}
	CGameObject* object = group->CGenericGroup::GetFirstElementInGroup();
	if (object == 0) {
		return 0;
	}
	p_position.m_xFixed = object->m_position.m_xFixed;
	p_position.m_yFixed = object->m_position.m_yFixed;
	p_position.m_zFixed = object->m_position.m_zFixed;
	return 1;
}

// FUNCTION: LEMBALL 0x00418640
void CPlayerLemmingGroupManager::ProcessDead()
{
}

// FUNCTION: LEMBALL 0x00418650
void CPlayerLemmingGroupManager::Process()
{
	bool controlledGroupDeleted = 0;
	CGenericGroup* genericGroup = CGenericGroupManager::GetFirstGroup();
	while (genericGroup != 0) {
		genericGroup->Process();
		genericGroup = CGenericGroupManager::GetNextGroup();
	}

	CPlayerLemmingGroup* group = (CPlayerLemmingGroup*) CGenericGroupManager::GetFirstGroup();
	while (group != 0) {
		CPlayerLemming* lemming = group->GetFirstDeadLemming();
		if (lemming != 0) {
			do {
				group->RemoveLemmingFromGroup(lemming);
				m_dead[m_deadCount] = lemming;
				m_deadCount++;
				if (group->GetElementsInGroup() == 0) {
					lemming = 0;
					if (GetPlayerControlledGroup() == group) {
						controlledGroupDeleted = 1;
					}
					DeleteGroup(group);
				}
				else {
					lemming = group->GetFirstDeadLemming();
				}
			} while (lemming != 0);
		}
		group = (CPlayerLemmingGroup*) CGenericGroupManager::GetNextGroup();
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
	CPlayerLemmingGroup* group = 0;
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
	if (group != 0) {
		return group->CheckPlayerControlled();
	}
	return 0;
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
			if (group != 0 && group->GetElementsInGroup() > 0) {
				((CPlayerLemmingGroup*) m_groups[m_controlledGroupIndex])->SetPlayerControlled(1, 0);
				return 1;
			}
			checked++;
		} while (checked < m_groupCount);
	}
	return 0;
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
			if (group != 0 && group->GetElementsInGroup() > 0) {
				((CPlayerLemmingGroup*) m_groups[m_controlledGroupIndex])->SetPlayerControlled(1, 0);
				return 1;
			}
			checked++;
		} while (checked < m_groupCount);
	}
	return 0;
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
			p_group->SetPlayerControlled(1, 0);
			return 1;
		}
		groups++;
		index++;
	} while (index < 8);
	return 0;
}

// FUNCTION: LEMBALL 0x004189c0
bool CPlayerLemmingGroupManager::MakeNoGroupsPlayerControlled()
{
	for (int i = 0; i < 8; i++) {
		if (m_groups[i] != 0) {
			((CPlayerLemmingGroup*) m_groups[i])->SetPlayerControlled(0, 0);
		}
	}
	return 1;
}

// FUNCTION: LEMBALL 0x004189f0
CPlayerLemmingGroup* CPlayerLemmingGroupManager::GetPlayerControlledGroup()
{
	int i = 0;
	if (m_groupCount > 0) {
		CGenericGroup** groups = m_groups;
		do {
			CPlayerLemmingGroup* group = (CPlayerLemmingGroup*) *groups;
			if (group != 0 && group->CheckPlayerControlled() == 1) {
				return (CPlayerLemmingGroup*) m_groups[i];
			}
			groups++;
			i++;
		} while (m_groupCount > i);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00418a30
void CPlayerLemmingGroupManager::AddNewWaypointToCurrentGroup(int p_x, int p_y)
{
	AiCoord coordinate(p_x << 12, p_y << 12, 0);
	CPlayerLemmingGroup* group = GetPlayerControlledGroup();
	if (group != 0) {
		group->AddNewWaypoint(coordinate, g_pGenericGroupFormationManager);
	}
}

// FUNCTION: LEMBALL 0x00418a90
void CPlayerLemmingGroupManager::RemoveWaypointsFromCurrentGroup()
{
	CPlayerLemmingGroup* group = GetPlayerControlledGroup();
	if (group != 0) {
		group->ClearExistingWaypoints();
	}
}

// FUNCTION: LEMBALL 0x00418ab0
void CPlayerLemmingGroupManager::UseObject(int p_objectId)
{
	CPlayerLemmingGroup* controlledGroup = GetPlayerControlledGroup();
	if (controlledGroup == 0) {
		return;
	}
	CGameObject* object = g_pObjects[(unsigned short) p_objectId];
	if (object->m_objectType != 2) {
		controlledGroup->AddUseObject(p_objectId);
		return;
	}
	CPlayerLemming* lemming = (CPlayerLemming*) object;
	if (lemming->m_action != 8) {
		CPlayerLemmingGroup* group = lemming->GetGroup();
		if (group != controlledGroup) {
			controlledGroup->SetPlayerControlled(0, 0);
		}
		group->SetPlayerControlled(1, lemming);
	}
}

// FUNCTION: LEMBALL 0x00418b20
void CPlayerLemmingGroupManager::ReformAlteredGroups(CPlayerLemmingGroup* p_excludedGroup)
{
	CPlayerLemmingGroup* group = (CPlayerLemmingGroup*) CGenericGroupManager::GetFirstGroup();
	while (group != 0) {
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
	if (group != 0) {
		CPlayerLemming* lemming = (CPlayerLemming*) group->CGenericGroup::GetFirstElementInGroup();
		while (lemming != 0) {
			lemming->RequestFire(p_x, p_y);
			lemming = (CPlayerLemming*) group->CGenericGroup::GetNextElementInGroup();
		}
	}
}

// FUNCTION: LEMBALL 0x00418ba0
void CPlayerLemmingGroupManager::GetPlayerStartPosition(AiCoord& p_position, int p_index)
{
	p_position.m_xFixed = m_startX[p_index] << 12;
	p_position.m_yFixed = m_startY[p_index] << 12;
	p_position.m_zFixed = m_startZ[p_index] << 12;
}

// FUNCTION: LEMBALL 0x00418be0
void CPlayerLemmingGroupManager::ConfigurePlayerLemmingCounts(int p_playerCount,
															  int p_count0,
															  int p_count1,
															  int p_count2,
															  int p_count3)
{
	m_startPositionCount = p_playerCount;
	if (p_count0 == -1) {
		m_lemmingCounts[0] = g_anDefaultPlayerLemmingCounts[p_playerCount][0];
		m_lemmingCounts[1] = g_anDefaultPlayerLemmingCounts[p_playerCount][1];
		m_lemmingCounts[2] = g_anDefaultPlayerLemmingCounts[p_playerCount][2];
		m_lemmingCounts[3] = g_anDefaultPlayerLemmingCounts[p_playerCount][3];
	}
	else {
		m_lemmingCounts[0] = p_count0;
		m_lemmingCounts[2] = p_count2;
		m_lemmingCounts[1] = p_count1;
		m_lemmingCounts[3] = p_count3;
	}
	for (int i = 0; i < p_playerCount; i++) {
		if (m_startX[i] > 1024 || m_startX[i] < 0) {
			m_startX[i] = i * 16;
		}
		if (m_startY[i] > 1024 || m_startY[i] < 0) {
			m_startY[i] = i * 16;
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
	if (g_pActiveConnection != 0) {
		CPlayerLemming** lemmings = m_networkLemmings;
		int remaining = 4;
		do {
			if (m_networkInitialized == 0) {
				*lemmings = new CPlayerLemming(0, 0, 0, 0, 1, 0);
			}
			(*lemmings)->Restart();
			CBaseObjectManager* manager = this;
			(*lemmings)->m_manager = manager;
			lemmings++;
			remaining--;
		} while (remaining != 0);
		m_networkInitialized = 1;
	}
}

// FUNCTION: LEMBALL 0x00418d20
void CPlayerLemmingGroupManager::LoadLevel(unsigned char* p_data, unsigned long p_dataSize, unsigned int p_skip)
{
	unsigned short* data = (unsigned short*) p_data;
	m_startPositionCount = 1;
	m_startX[0] = *data++;
	m_startY[0] = *data++;
	m_startZ[0] = *data++;
	int x = m_startX[0];
	int y = m_startY[0];
	CMap* map = g_pMap;
	int blockX = x >> 4;
	int blockY = y >> 4;
	unsigned short z;
	if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && map->m_ground.m_height > blockY) {
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(x & 0xf, y & 0xf);
	}
	else {
		z = 0;
	}
	m_startZ[0] = z;
	for (int i = 1; i < 4; i++) {
		m_startX[i] = m_startX[0];
		m_startY[i] = m_startY[0];
		m_startZ[i] = m_startZ[0];
	}
	CPlayerLemming** reuse = 0;
	int count = g_pGenericGroupAI->m_lemmingCount;
	int dead = 4 - count;
	if (p_skip != 0) {
		reuse = g_pGenericGroupAI->m_networkLemmings;
	}
	m_deadCount = 0;
	for (i = 0; i < dead; i++) {
		CPlayerLemming* lemming;
		if (reuse == 0) {
			lemming = new CPlayerLemming(m_startX[i], m_startY[i], m_startZ[i], 0, 0, 0);
		}
		else {
			lemming = *reuse++;
		}
		lemming->Restart();
		CBaseObjectManager* manager = this;
		lemming->m_manager = manager;
		lemming->m_action = ACTION_8;
		int& objectCount = g_pGenericGroupAI->m_objectCount;
		for (int j = 0; j < objectCount; j++) {
			CGameObject**& objects = g_pGenericGroupAI->m_objects;
			if (objects[j] == lemming) {
				objectCount--;
				for (; j < objectCount; j++) {
					objects[j] = objects[j + 1];
				}
				objects[objectCount] = 0;
				break;
			}
		}
		g_wLemmingCount--;
		m_dead[m_deadCount++] = lemming;
	}
	CPlayerLemmingGroup* group = (CPlayerLemmingGroup*) CGenericGroupManager::GetFirstGroup();
	for (i = 0; i < count; i++) {
		int delay = (3900 + i * 800) / 50;
		CPlayerLemming* lemming;
		if (reuse == 0) {
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
	unsigned int doorTime = (count * 800 + 3900) / 50;
	if (g_pGenericGroupAI->m_unk0x5c < doorTime) {
		g_pGenericGroupAI->m_unk0x5c = doorTime;
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
	for (int i = 0; i < m_startPositionCount; i++) {
		m_startX[i] = *data++;
		m_startY[i] = *data++;
		m_startZ[i] = *data++;
		int x = m_startX[i];
		int y = m_startY[i];
		int blockX = x >> 4;
		int blockY = y >> 4;
		unsigned short z;
		if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && map->m_ground.m_height > blockY) {
			z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(x & 0xf, y & 0xf);
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
	CPlayerLemming** reuse = 0;
	if (p_skip != 0) {
		reuse = g_pGenericGroupAI->m_networkLemmings;
	}
	m_deadCount = 0;
	for (i = 0; i < dead; i++) {
		CPlayerLemming* lemming;
		if (reuse == 0) {
			lemming = new CPlayerLemming(m_startX[i], m_startY[i], m_startZ[i], 0, 0, 0);
		}
		else {
			lemming = *reuse++;
		}
		lemming->Restart();
		lemming->m_action = ACTION_8;
		int& objectCount = g_pGenericGroupAI->m_objectCount;
		for (int j = 0; j < objectCount; j++) {
			CGameObject**& objects = g_pGenericGroupAI->m_objects;
			if (objects[j] == lemming) {
				objectCount--;
				for (; j < objectCount; j++) {
					objects[j] = objects[j + 1];
				}
				objects[objectCount] = 0;
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
			int delay = (3900 + j * 800) / 50;
			CPlayerLemming* lemming;
			if (reuse == 0) {
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
		unsigned int doorTime = (m_lemmingCounts[i] * 800 + 3900) / 50;
		if (g_pGenericGroupAI->m_unk0x5c < doorTime) {
			g_pGenericGroupAI->m_unk0x5c = doorTime;
		}
		doorTime = (m_lemmingCounts[i] * 800 + 4100) / 50;
		if (p_skip == 0) {
			g_pGenericGroupAI->AddNewTrapDoor(m_startX[i], m_startY[i], m_startZ[i], doorTime);
		}
	}
	MakeParticularGroupPlayerControlled((CPlayerLemmingGroup*) CGenericGroupManager::GetFirstGroup());
}

// FUNCTION: LEMBALL 0x00419440
bool CPlayerLemmingGroupManager::HasSfxChanged()
{
	int changed = 0;
	int i = 0;
	if (m_groupCount > 0) {
		CGenericGroup** group = m_groups;
		do {
			if (((CPlayerLemmingGroup*) *group)->HasSfxChanged() != 0 || changed != 0) {
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
	if (g_pActiveConnection != 0) {
		count = 4;
		for (int i = 0; i < 4; i++) {
			m_networkLemmings[i]->GetViewData(*p_viewData++);
		}
	}
	return CGenericGroupManager::GetViewData(p_viewData) + count;
}
