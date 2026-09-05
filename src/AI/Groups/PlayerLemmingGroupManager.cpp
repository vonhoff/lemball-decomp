#include "PlayerLemmingGroupManager.h"

#include "../../Map/Base/Map.h"
#include "../Navigation/Ai.h"
#include "../Objects/PlayerLemming.h"
#include "PlayerLemmingGroup.h"

// 68K 0x1060f6e4 __ct__26CPlayerLemmingGroupManagerFP3CAIP14CObjectManagerP17CFormationManager
// STUB: LEMBALL 0x00418400
PlayerLemmingGroupManager::PlayerLemmingGroupManager(Ai* p_arg0, ObjectManager* p_arg1, FormationManager* p_arg2)
	: GenericGroupManager(p_arg0, p_arg1, p_arg2), BaseObjectManager(0x22, 0x17)
{
}

// 68K 0x1060f84a Restart__26CPlayerLemmingGroupManagerFv
// FUNCTION: LEMBALL 0x00418520
void PlayerLemmingGroupManager::Restart()
{
	GenericGroupManager::Restart();
	m_controlledGroupIndex = 0;
}

// 68K 0x1060f9ac GetDead__26CPlayerLemmingGroupManagerFv
// FUNCTION: LEMBALL 0x004185d0
PlayerLemming* PlayerLemmingGroupManager::GetDead()
{
	if (m_deadCount == 0) {
		return 0;
	}
	m_deadCount--;
	return m_dead[m_deadCount];
}

// 68K 0x1060fa00 GetLeaderPos__26CPlayerLemmingGroupManagerFR7AICOORD
// FUNCTION: LEMBALL 0x004185f0
bool PlayerLemmingGroupManager::GetLeaderPos(AiCoord& p_position)
{
	PlayerLemmingGroup* group = GetPlayerControlledGroup();
	if (group == 0) {
		return 0;
	}
	GameObject* object = group->GenericGroup::GetFirstElementInGroup();
	if (object == 0) {
		return 0;
	}
	p_position.m_xFixed = object->m_position.m_xFixed;
	p_position.m_yFixed = object->m_position.m_yFixed;
	p_position.m_zFixed = object->m_position.m_zFixed;
	return 1;
}

// 68K 0x1060fa8a ProcessDead__26CPlayerLemmingGroupManagerFv
// FUNCTION: LEMBALL 0x00418640
void PlayerLemmingGroupManager::ProcessDead()
{
}

// 68K 0x1060fac2 Process__26CPlayerLemmingGroupManagerFv
// STUB: LEMBALL 0x00418650
void PlayerLemmingGroupManager::Process()
{
}

// 68K 0x1060fbd4 DeleteGroup__26CPlayerLemmingGroupManagerFP19CPlayerLemmingGroup
// FUNCTION: LEMBALL 0x00418720
void PlayerLemmingGroupManager::DeleteGroup(PlayerLemmingGroup* p_group)
{
	p_group->Restart();
}

// 68K 0x1060fc30 CreateNewGroup__26CPlayerLemmingGroupManagerFUsPUs
// STUB: LEMBALL 0x00418730
void PlayerLemmingGroupManager::CreateNewGroup(unsigned short p_count, unsigned short* p_objectIds)
{
}

// 68K 0x1060fd36 AddPlayerLemmingToGroup__26CPlayerLemmingGroupManagerFP14CPlayerLemmingP19CPlayerLemmingGroup
// FUNCTION: LEMBALL 0x00418820
void PlayerLemmingGroupManager::AddPlayerLemmingToGroup(PlayerLemming* p_lemming, PlayerLemmingGroup* p_group)
{
	GenericGroupManager::FindElementInGroupAndRemoveIt(p_lemming);
	p_group->AddLemmingToGroup(p_lemming);
}

// 68K 0x1060fdc4 IsLemmingPlayerControlled__26CPlayerLemmingGroupManagerFP14CPlayerLemming
// FUNCTION: LEMBALL 0x00418840
bool PlayerLemmingGroupManager::IsLemmingPlayerControlled(PlayerLemming* p_lemming)
{
	PlayerLemmingGroup* group =
		(PlayerLemmingGroup*) GenericGroupManager::GetGroupElementIsMemberOf((GameObject*) p_lemming);
	if (group != 0) {
		return group->CheckPlayerControlled();
	}
	return 0;
}

// 68K 0x1060fe42 MakeNextGroupPlayerControlled__26CPlayerLemmingGroupManagerFv
// STUB: LEMBALL 0x00418860
bool PlayerLemmingGroupManager::MakeNextGroupPlayerControlled()
{
	return 0;
}

// 68K 0x1060ff02 MakePreviousGroupPlayerControlled__26CPlayerLemmingGroupManagerFv
// STUB: LEMBALL 0x004188e0
bool PlayerLemmingGroupManager::MakePreviousGroupPlayerControlled()
{
	return 0;
}

// 68K 0x1060ffce MakeParticularGroupPlayerControlled__26CPlayerLemmingGroupManagerFP19CPlayerLemmingGroup
// STUB: LEMBALL 0x00418960
bool PlayerLemmingGroupManager::MakeParticularGroupPlayerControlled(PlayerLemmingGroup* p_group)
{
	return 0;
}

// 68K 0x10610098 MakeNoGroupsPlayerControlled__26CPlayerLemmingGroupManagerFv
// FUNCTION: LEMBALL 0x004189c0
bool PlayerLemmingGroupManager::MakeNoGroupsPlayerControlled()
{
	for (int i = 0; i < 8; i++) {
		if (m_groups[i] != 0) {
			((PlayerLemmingGroup*) m_groups[i])->SetPlayerControlled(0, 0);
		}
	}
	return 1;
}

// 68K 0x10610112 GetPlayerControlledGroup__26CPlayerLemmingGroupManagerFv
// FUNCTION: LEMBALL 0x004189f0
PlayerLemmingGroup* PlayerLemmingGroupManager::GetPlayerControlledGroup()
{
	int i = 0;
	if (m_groupCount > 0) {
		GenericGroup** groups = m_groups;
		do {
			PlayerLemmingGroup* group = (PlayerLemmingGroup*) *groups;
			if (group != 0 && group->CheckPlayerControlled() == 1) {
				return (PlayerLemmingGroup*) m_groups[i];
			}
			i++;
			groups++;
		} while (m_groupCount > i);
	}
	return 0;
}

// 68K 0x1061018c AddNewWaypointToCurrentGroup__26CPlayerLemmingGroupManagerFii
// STUB: LEMBALL 0x00418a30
void PlayerLemmingGroupManager::AddNewWaypointToCurrentGroup(int p_x, int p_y)
{
}

// 68K 0x1061025e RemoveWaypointsFromCurrentGroup__26CPlayerLemmingGroupManagerFv
// FUNCTION: LEMBALL 0x00418a90
void PlayerLemmingGroupManager::RemoveWaypointsFromCurrentGroup()
{
	PlayerLemmingGroup* group = GetPlayerControlledGroup();
	if (group != 0) {
		group->ClearExistingWaypoints();
	}
}

// 68K 0x106102ce UseObject__26CPlayerLemmingGroupManagerFi
// STUB: LEMBALL 0x00418ab0
void PlayerLemmingGroupManager::UseObject(int p_objectId)
{
}

// 68K 0x1061038e ReformAlteredGroups__26CPlayerLemmingGroupManagerFP19CPlayerLemmingGroup
// STUB: LEMBALL 0x00418b20
void PlayerLemmingGroupManager::ReformAlteredGroups(PlayerLemmingGroup* p_excludedGroup)
{
}

// 68K 0x10601042c PlayerGroupRequestFire__26CPlayerLemmingGroupManagerFii
// FUNCTION: LEMBALL 0x00418b60
void PlayerLemmingGroupManager::PlayerGroupRequestFire(int p_x, int p_y)
{
	PlayerLemmingGroup* group = GetPlayerControlledGroup();
	if (group != 0) {
		PlayerLemming* lemming = (PlayerLemming*) group->GenericGroup::GetFirstElementInGroup();
		while (lemming != 0) {
			lemming->RequestFire(p_x, p_y);
			lemming = (PlayerLemming*) group->GenericGroup::GetNextElementInGroup();
		}
	}
}

// 68K 0x106104be InitialiseNetwork__26CPlayerLemmingGroupManagerFv
// STUB: LEMBALL 0x00418ca0
void PlayerLemmingGroupManager::InitialiseNetwork()
{
}

// 68K 0x106105a0 LoadLevel__26CPlayerLemmingGroupManagerFPUcUlUc
// FUNCTION: LEMBALL 0x00418d20
void PlayerLemmingGroupManager::LoadLevel(unsigned char* p_data, unsigned long p_dataSize, unsigned char p_skip)
{
	unsigned short* data = (unsigned short*) p_data;
	m_startPositionCount = 1;
	m_startX[0] = *data++;
	m_startY[0] = *data++;
	m_startZ[0] = *data++;
	int x = m_startX[0];
	int y = m_startY[0];
	Map* map = g_pMap;
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
	PlayerLemming** reuse = 0;
	int count = g_pGenericGroupAI->m_lemmingCount;
	int dead = 4 - count;
	if (*(unsigned int*) &p_skip != 0) {
		reuse = g_pGenericGroupAI->m_networkLemmings;
	}
	m_deadCount = 0;
	for (i = 0; i < dead; i++) {
		PlayerLemming* lemming;
		if (reuse == 0) {
			lemming = new PlayerLemming(m_startX[i], m_startY[i], m_startZ[i], 0, 0, 0);
		}
		else {
			lemming = *reuse++;
		}
		lemming->Restart();
		BaseObjectManager* manager = this;
		lemming->m_manager = manager;
		lemming->m_action = (eAction) 8;
		int& objectCount = g_pGenericGroupAI->m_objectCount;
		for (int j = 0; j < objectCount; j++) {
			GameObject**& objects = g_pGenericGroupAI->m_objects;
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
	PlayerLemmingGroup* group = (PlayerLemmingGroup*) GenericGroupManager::GetFirstGroup();
	for (i = 0; i < count; i++) {
		int delay = (3900 + i * 800) / 50;
		PlayerLemming* lemming;
		if (reuse == 0) {
			lemming = new PlayerLemming(m_startX[i], m_startY[i], m_startZ[i], 0, 0, delay);
		}
		else {
			lemming = *reuse++;
		}
		lemming->Restart();
		BaseObjectManager* manager = this;
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
	if (*(unsigned int*) &p_skip == 0) {
		g_pGenericGroupAI->AddNewTrapDoor(m_startX[0], m_startY[0], m_startZ[0], doorTime);
	}
}

// 68K 0x106108a8 LoadAdditionalPlayerStartPositions__26CPlayerLemmingGroupManagerFPUcUlUc
// FUNCTION: LEMBALL 0x00419060
void PlayerLemmingGroupManager::LoadAdditionalPlayerStartPositions(unsigned char* p_data,
																   unsigned long p_dataSize,
																   unsigned char p_skip)
{
	Map* map = g_pMap;
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
	PlayerLemming** reuse = 0;
	if (*(unsigned int*) &p_skip != 0) {
		reuse = g_pGenericGroupAI->m_networkLemmings;
	}
	m_deadCount = 0;
	for (i = 0; i < dead; i++) {
		PlayerLemming* lemming;
		if (reuse == 0) {
			lemming = new PlayerLemming(m_startX[i], m_startY[i], m_startZ[i], 0, 0, 0);
		}
		else {
			lemming = *reuse++;
		}
		lemming->Restart();
		lemming->m_action = (eAction) 8;
		int& objectCount = g_pGenericGroupAI->m_objectCount;
		for (int j = 0; j < objectCount; j++) {
			GameObject**& objects = g_pGenericGroupAI->m_objects;
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
		BaseObjectManager* manager = this;
		lemming->m_manager = manager;
		m_dead[m_deadCount++] = lemming;
	}
	for (i = 0; i < m_startPositionCount; i++) {
		for (int j = 0; j < m_lemmingCounts[i]; j++) {
			int delay = (3900 + j * 800) / 50;
			PlayerLemming* lemming;
			if (reuse == 0) {
				lemming = new PlayerLemming(m_startX[i], m_startY[i], m_startZ[i], 0, 0, delay);
			}
			else {
				lemming = *reuse++;
			}
			lemming->Restart();
			BaseObjectManager* manager = this;
			lemming->m_manager = manager;
			PlayerLemmingGroup* group = (PlayerLemmingGroup*) GenericGroupManager::GetNthGroup(i);
			AddPlayerLemmingToGroup(lemming, group);
		}
		unsigned int doorTime = (m_lemmingCounts[i] * 800 + 3900) / 50;
		if (g_pGenericGroupAI->m_unk0x5c < doorTime) {
			g_pGenericGroupAI->m_unk0x5c = doorTime;
		}
		doorTime = (m_lemmingCounts[i] * 800 + 4100) / 50;
		if (*(unsigned int*) &p_skip == 0) {
			g_pGenericGroupAI->AddNewTrapDoor(m_startX[i], m_startY[i], m_startZ[i], doorTime);
		}
	}
	MakeParticularGroupPlayerControlled((PlayerLemmingGroup*) GenericGroupManager::GetFirstGroup());
}

// 68K 0x10610c54 HasSFXChanged__26CPlayerLemmingGroupManagerFv
// FUNCTION: LEMBALL 0x00419440
bool PlayerLemmingGroupManager::HasSfxChanged()
{
	int changed = 0;
	for (int i = 0; i < m_groupCount; i++) {
		if (((PlayerLemmingGroup*) m_groups[i])->HasSfxChanged() != 0 || changed != 0) {
			changed = 1;
		}
		else {
			changed = 0;
		}
	}
	return changed;
}

// 68K 0x10610cc6 GetViewData__26CPlayerLemmingGroupManagerFP9CViewData
// STUB: LEMBALL 0x00419490
int PlayerLemmingGroupManager::GetViewData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x1060f890 __dt__26CPlayerLemmingGroupManagerFv
PlayerLemmingGroupManager::~PlayerLemmingGroupManager()
{
}
