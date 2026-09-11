#include "PlayerLemmingGroup.h"

#include "../../Visos/Network/Connect.h"
#include "../Navigation/AiDestinationEntry.h"
#include "../Navigation/AiDestinationList.h"
#include "../Objects/PlayerLemming.h"
#include "../Objects/ViewData.h"

// 68K 0x1060e81a __ct__19CPlayerLemmingGroupFP3CAIP14CObjectManagerP17CFormationManager
// FUNCTION: LEMBALL 0x00414010
PlayerLemmingGroup::PlayerLemmingGroup(Ai* p_ai, ObjectManager* p_objectManager, FormationManager* p_formationManager)
	: GenericGroup(p_ai, p_objectManager, p_formationManager)
{
}

// 68K 0x1060e89e Restart__19CPlayerLemmingGroupFv
// FUNCTION: LEMBALL 0x00414040
void PlayerLemmingGroup::Restart()
{
	GenericGroup::Restart();
	m_formationIndex = 0;
	m_altered = 0;
	m_playerControlled = 0;
	m_useObject = 0;
}

// 68K 0x1060e946 GetViewData__19CPlayerLemmingGroupFP9CViewData
// FUNCTION: LEMBALL 0x00414080
int PlayerLemmingGroup::GetViewData(ViewData* p_viewData)
{
	int count;
	PlayerLemmingGroup* self = this;
	GameObject* object = self->GetFirstElementInGroup();
	count = 0;
	if (object != 0) {
		do {
			if (object->m_action != (eAction) 0xc) {
				object->GetViewData(*p_viewData);
				p_viewData++;
				count++;
			}
			object = self->GetNextElementInGroup();
		} while (object != 0);
	}
	return count;
}

// 68K 0x1060ea70 Delete__19CPlayerLemmingGroupFv
// FUNCTION: LEMBALL 0x00414130
void PlayerLemmingGroup::Delete()
{
	if (m_useObject != 0) {
		m_useObject->m_unk0x8c = 0;
	}
	m_useObject = 0;
}

// 68K 0x1060eab2 Process__19CPlayerLemmingGroupFv
// STUB: LEMBALL 0x00414150
bool PlayerLemmingGroup::Process()
{
	return 0;
}

// 68K 0x1060f0f2 AddLemmingToGroup__19CPlayerLemmingGroupFP14CPlayerLemming
// FUNCTION: LEMBALL 0x00414600
bool PlayerLemmingGroup::AddLemmingToGroup(PlayerLemming* p_lemming)
{
	GenericGroup::AddElementToGroup(p_lemming);
	m_altered = 1;
	p_lemming->SetGroup(this);
	if (GetElementsInGroup() == 0 && m_playerControlled == 1) {
		p_lemming->SetGroupLeader(1);
	}
	else {
		p_lemming->SetGroupLeader(0);
	}
	return true;
}

// 68K 0x1060f19e AddUseObject__19CPlayerLemmingGroupFi
// FUNCTION: LEMBALL 0x00414660
void PlayerLemmingGroup::AddUseObject(int p_objectId)
{
	unsigned short count;
	AiDestinationList* list;
	GameObject* p_object = 0;
	int objectCount = g_wObjectCount;
	for (unsigned int i = 0; (int) i < objectCount; i++) {
		p_object = g_pObjects[(unsigned short) i];
		if (p_object != 0 && (unsigned int) p_object->m_objectId == (unsigned int) p_objectId) {
			break;
		}
	}
	AiCoord position = p_object->ActivatePosition();
	list = m_destinationList;
	count = list->m_count;
	if (count < list->m_capacity) {
		list->m_count = count + 1;
		AiDestinationEntry* entry = &list->m_entries[count];
		entry->m_type = (eDestinationType) 1;
		entry->m_coordinate.m_xFixed = position.m_xFixed;
		entry->m_coordinate.m_yFixed = position.m_yFixed;
		entry->m_coordinate.m_zFixed = position.m_zFixed;
	}
	AiDestinationList* useList = m_destinationList;
	unsigned short useCount = useList->m_count;
	if (useCount < useList->m_capacity) {
		useList->m_count = useCount + 1;
		AiDestinationEntry* entry = &useList->m_entries[useCount];
		entry->m_type = (eDestinationType) 2;
		entry->m_coordinate.m_xFixed = position.m_xFixed;
		entry->m_coordinate.m_yFixed = position.m_yFixed;
		entry->m_coordinate.m_zFixed = position.m_zFixed;
		entry->m_metadata = (unsigned short) p_objectId;
	}
}

// 68K 0x1060f2a6 AddUseObject__19CPlayerLemmingGroupFP11CGameObjecti
// FUNCTION: LEMBALL 0x00414730
void PlayerLemmingGroup::AddUseObject(GameObject* p_object, int p_objectId)
{
	unsigned short count;
	AiDestinationList* list;
	AiCoord position = p_object->ActivatePosition();
	list = m_destinationList;
	count = list->m_count;
	if (count < list->m_capacity) {
		list->m_count = count + 1;
		AiDestinationEntry* entry = &list->m_entries[count];
		entry->m_type = (eDestinationType) 1;
		entry->m_coordinate.m_xFixed = position.m_xFixed;
		entry->m_coordinate.m_yFixed = position.m_yFixed;
		entry->m_coordinate.m_zFixed = position.m_zFixed;
	}
	list = m_destinationList;
	count = list->m_count;
	if (count < list->m_capacity) {
		list->m_count = count + 1;
		AiDestinationEntry* entry = &list->m_entries[count];
		entry->m_type = (eDestinationType) 2;
		entry->m_coordinate.m_xFixed = position.m_xFixed;
		entry->m_coordinate.m_yFixed = position.m_yFixed;
		entry->m_coordinate.m_zFixed = position.m_zFixed;
		entry->m_metadata = (unsigned short) p_objectId;
	}
}

// 68K 0x1060f38c RemoveLemmingFromGroup__19CPlayerLemmingGroupFP14CPlayerLemming
// FUNCTION: LEMBALL 0x004147d0
bool PlayerLemmingGroup::RemoveLemmingFromGroup(PlayerLemming* p_lemming)
{
	GenericGroup::RemoveElementFromGroup(p_lemming);
	PlayerLemming* leader = (PlayerLemming*) GenericGroup::GetFirstElementInGroup();
	if (m_playerControlled == 1 && leader != 0) {
		leader->SetGroupLeader(1);
	}
	m_altered = 1;
	return 1;
}

// 68K 0x1060f420 SetPlayerControlled__19CPlayerLemmingGroupFUcP14CPlayerLemming
// FUNCTION: LEMBALL 0x00414810
void PlayerLemmingGroup::SetPlayerControlled(undefined4 p_playerControlled, PlayerLemming* p_leader)
{
	PlayerLemming* first = (PlayerLemming*) GenericGroup::GetFirstElementInGroup();
	PlayerLemming* lemming = first;
	if (first != 0) {
		do {
			lemming->SetGroup(p_playerControlled);
			lemming->SetGroupLeader(0);
			lemming = (PlayerLemming*) GenericGroup::GetNextElementInGroup();
		} while (lemming != 0);
	}
	if (p_leader == 0) {
		p_leader = first;
	}
	m_playerControlled = p_playerControlled;
	if (p_leader != 0) {
		p_leader->SetGroupLeader(1);
		if (p_leader != first) {
			GenericGroup::SwapElements(p_leader, first);
		}
	}
}

// 68K 0x1060f4e4 CheckPlayerControlled__19CPlayerLemmingGroupFv
// FUNCTION: LEMBALL 0x00414880
bool PlayerLemmingGroup::CheckPlayerControlled()
{
	return m_playerControlled;
}

// 68K 0x1060f526 GetFirstDeadLemming__19CPlayerLemmingGroupFv
// FUNCTION: LEMBALL 0x00414890
PlayerLemming* PlayerLemmingGroup::GetFirstDeadLemming()
{
	PlayerLemming* lemming = (PlayerLemming*) GenericGroup::GetFirstElementInGroup();
	while (1) {
		if (lemming == 0) {
			return 0;
		}
		if (lemming->m_action == 8) {
			break;
		}
		lemming = (PlayerLemming*) GenericGroup::GetNextElementInGroup();
	}
	return lemming;
}

// 68K 0x1060f596 ClearExistingWaypoints__19CPlayerLemmingGroupFv
// FUNCTION: LEMBALL 0x004148f0
void PlayerLemmingGroup::ClearExistingWaypoints()
{
	GenericGroup::ClearExistingWaypoints();
	if (m_useObject != 0) {
		if (GetGroupState() == (eGroupState) 3 && m_useObject->m_heading != 0 && m_useObject->m_unk0x8c != 0) {
			if (g_pActiveConnection != 0) {
				m_useObject->SendCancel();
			}
			m_useObject->m_unk0x8c = 0;
		}
		m_currentUseElement = GetElementsInGroup();
	}
}

// 68K 0x1060f66a HasSFXChanged__19CPlayerLemmingGroupFv
// FUNCTION: LEMBALL 0x004149a0
bool PlayerLemmingGroup::HasSfxChanged()
{
	bool changed = false;
	PlayerLemming* lemming = (PlayerLemming*) GenericGroup::GetFirstElementInGroup();
	while (lemming != 0) {
		changed = lemming->CheckSfx() || changed;
		lemming = (PlayerLemming*) GenericGroup::GetNextElementInGroup();
	}
	return changed;
}

// 68K 0x1060e8ec __dt__19CPlayerLemmingGroupFv
PlayerLemmingGroup::~PlayerLemmingGroup()
{
}
