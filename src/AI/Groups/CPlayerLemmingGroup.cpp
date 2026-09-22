#include "CPlayerLemmingGroup.h"

#include "../../Visos/Foundation/CVSMath.h"
#include "../../Visos/Network/CConnect.h"
#include "../Base/CGlobalGameObject.h"
#include "../Managers/CObjectManager.h"
#include "../Navigation/CAiDestinationEntry.h"
#include "../Navigation/CAiDestinationList.h"
#include "../Objects/CPlayerLemming.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Groups/CGenericGroup.h"

// FUNCTION: LEMBALL 0x00414010
CPlayerLemmingGroup::CPlayerLemmingGroup(CAI* p_ai,
										 CObjectManager* p_objectManager,
										 CFormationManager* p_formationManager)
	: CGenericGroup(p_ai, p_objectManager, p_formationManager)
{
}

// FUNCTION: LEMBALL 0x00414040
void CPlayerLemmingGroup::Restart()
{
	CGenericGroup::Restart();
	m_formationIndex = 0;
	m_altered = 0;
	m_playerControlled = 0;
	m_useObject = 0;
}

// FUNCTION: LEMBALL 0x00414070
CPlayerLemmingGroup::~CPlayerLemmingGroup()
{
}

// FUNCTION: LEMBALL 0x00414080
int CPlayerLemmingGroup::GetViewData(CViewData* p_viewData)
{
	int count;
	CPlayerLemmingGroup* self = this;
	CGameObject* object = self->GetFirstElementInGroup();
	count = 0;
	if (object != 0) {
		do {
			if (object->m_action != ACTION_12) {
				object->GetViewData(*p_viewData);
				p_viewData++;
				count++;
			}
			object = self->GetNextElementInGroup();
		} while (object != 0);
	}
	return count;
}

// FUNCTION: LEMBALL 0x00414130
void CPlayerLemmingGroup::Delete()
{
	if (m_useObject != 0) {
		m_useObject->m_activationReserved = 0;
	}
	m_useObject = 0;
}

// FUNCTION: LEMBALL 0x00414150
bool CPlayerLemmingGroup::Process()
{
	int count = 0;
	int moving = 0;
	AiCoord position;
	AiCoord memberPosition;
	CGenericGroup::CalculateBoundingBox(0x18);
	CGameObject* member = CGenericGroup::GetFirstElementInGroup();
	while (member != 0) {
		count++;
		member->Process();
		if (member->DestinationExists()) {
			moving++;
		}
		member = CGenericGroup::GetNextElementInGroup();
	}
	if (count > 0 && moving == 0) {
		switch (GetGroupState()) {
		case 0: {
			CAiDestinationList* list = m_destinationList;
			if (list->m_count > 0) {
				CAiDestinationEntry entry = list->PopFirst();
				switch (entry.m_type) {
				case 1:
					SendNewWaypoint(entry.GetCoordinate());
					break;
				case 2: {
					int id = entry.m_metadata;
					CGameObject* object = g_pGroupObjectManager->FindObject(id);
					if (object != 0) {
						if (object->m_activationReserved != 0) {
							const AiCoord& activation = object->ActivatePosition();
							position.m_xFixed = activation.m_xFixed;
							position.m_yFixed = activation.m_yFixed;
							position.m_zFixed = activation.m_zFixed;
							list = m_destinationList;
							if (list->m_capacity > list->m_count) {
								list->PrependSlot();
								CAiDestinationEntry* dest = list->m_entries;
								dest->m_type = DESTINATION_OBJECT;
								dest->m_coordinate.m_xFixed = position.m_xFixed;
								dest->m_coordinate.m_yFixed = position.m_yFixed;
								dest->m_coordinate.m_zFixed = position.m_zFixed;
								dest->m_metadata = id;
							}
						}
						else {
							member = CGenericGroup::GetNthElementInGroup(0);
							if (member != 0) {
								const AiCoord& activation = object->ActivatePosition();
								position.m_xFixed = activation.m_xFixed;
								position.m_yFixed = activation.m_yFixed;
								position.m_zFixed = activation.m_zFixed;
								{
									int y = member->m_position.m_yFixed;
									int z = member->m_position.m_zFixed;
									int x = member->m_position.m_xFixed;
									memberPosition.m_xFixed = x;
									memberPosition.m_yFixed = y;
									memberPosition.m_zFixed = z;
								}
								if (CloseTo(memberPosition, position)) {
									m_useObject = object;
									object->m_activationReserved = 1;
									m_currentUseElement = 0;
									if (m_useObject->Activate(member)) {
										SetGroupState(GROUP_STATE_USING_OBJECT);
									}
									else {
										m_useObject->m_activationReserved = 0;
									}
								}
								else {
									AddUseObject(object, entry.m_metadata);
								}
							}
						}
					}
					break;
				}
				}
			}
			break;
		}
		case 1:
			member = CGenericGroup::GetNthElementInGroup(m_currentUseElement);
			if (member == 0) {
				SetGroupState(GROUP_STATE_IDLE);
				m_useObject->m_activationReserved = 0;
				m_useObject = 0;
			}
			else {
				switch (m_useObject->Usage()) {
				case 1:
					m_currentUseElement++;
					{
						int y = member->m_position.m_yFixed;
						int z = member->m_position.m_zFixed;
						int x = member->m_position.m_xFixed;
						position.m_xFixed = x;
						position.m_yFixed = y;
						position.m_zFixed = z;
					}
					member = CGenericGroup::GetNextElementInGroup();
					while (member != 0) {
						member->AddDestination(position);
						{
							int y = member->m_position.m_yFixed;
							int z = member->m_position.m_zFixed;
							int x = member->m_position.m_xFixed;
							position.m_xFixed = x;
							position.m_yFixed = y;
							position.m_zFixed = z;
						}
						member = CGenericGroup::GetNextElementInGroup();
					}
					SetGroupState(GROUP_STATE_ATTACKING);
					break;
				case 2:
					SetGroupState(GROUP_STATE_IDLE);
					m_useObject->m_activationReserved = 0;
					m_useObject = 0;
					break;
				}
			}
			break;
		case 2:
			if (m_useObject->m_action == ACTION_0x18 && moving == 0) {
				if (GetElementsInGroup() <= m_currentUseElement) {
					SetGroupState(GROUP_STATE_IDLE);
					m_useObject->m_activationReserved = 0;
					m_useObject = 0;
				}
				else {
					member = CGenericGroup::GetNthElementInGroup(m_currentUseElement);
					m_useObject->Activate(member);
					SetGroupState(GROUP_STATE_USING_OBJECT);
				}
			}
			break;
		case 3:
			switch (m_useObject->UsableState()) {
			case 1:
				m_useObject->m_activationReserved = 0;
				if (m_useObject->m_heading != 0) {
					const AiCoord& activation = m_useObject->ActivatePosition();
					position.m_xFixed = activation.m_xFixed;
					position.m_yFixed = activation.m_yFixed;
					position.m_zFixed = activation.m_zFixed;
					int id = m_useObject->m_objectId;
					CAiDestinationList* list = m_destinationList;
					if (list->m_capacity > list->m_count) {
						list->PrependSlot();
						CAiDestinationEntry* dest = list->m_entries;
						dest->m_type = DESTINATION_OBJECT;
						dest->m_coordinate.m_xFixed = position.m_xFixed;
						dest->m_coordinate.m_yFixed = position.m_yFixed;
						dest->m_coordinate.m_zFixed = position.m_zFixed;
						dest->m_metadata = id;
					}
					SetGroupState(GROUP_STATE_IDLE);
				}
				break;
			case 2:
				SetGroupState(GROUP_STATE_MOVING);
				break;
			}
			break;
		}
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00414600
bool CPlayerLemmingGroup::AddLemmingToGroup(CPlayerLemming* p_lemming)
{
	CGenericGroup::AddElementToGroup(p_lemming);
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

// FUNCTION: LEMBALL 0x00414660
void CPlayerLemmingGroup::AddUseObject(int p_objectId)
{
	unsigned short count;
	CAiDestinationList* list;
	CGameObject* p_object = 0;
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
		CAiDestinationEntry* entry = &list->m_entries[count];
		entry->m_type = DESTINATION_COORD;
		entry->m_coordinate.m_xFixed = position.m_xFixed;
		entry->m_coordinate.m_yFixed = position.m_yFixed;
		entry->m_coordinate.m_zFixed = position.m_zFixed;
	}
	CAiDestinationList* useList = m_destinationList;
	unsigned short useCount = useList->m_count;
	if (useCount < useList->m_capacity) {
		useList->m_count = useCount + 1;
		CAiDestinationEntry* entry = &useList->m_entries[useCount];
		entry->m_type = DESTINATION_OBJECT;
		entry->m_coordinate.m_xFixed = position.m_xFixed;
		entry->m_coordinate.m_yFixed = position.m_yFixed;
		entry->m_coordinate.m_zFixed = position.m_zFixed;
		entry->m_metadata = (unsigned short) p_objectId;
	}
}

// FUNCTION: LEMBALL 0x00414730
void CPlayerLemmingGroup::AddUseObject(CGameObject* p_object, int p_objectId)
{
	unsigned short count;
	CAiDestinationList* list;
	AiCoord position = p_object->ActivatePosition();
	list = m_destinationList;
	count = list->m_count;
	if (count < list->m_capacity) {
		list->m_count = count + 1;
		CAiDestinationEntry* entry = &list->m_entries[count];
		entry->m_type = DESTINATION_COORD;
		entry->m_coordinate.m_xFixed = position.m_xFixed;
		entry->m_coordinate.m_yFixed = position.m_yFixed;
		entry->m_coordinate.m_zFixed = position.m_zFixed;
	}
	list = m_destinationList;
	count = list->m_count;
	if (count < list->m_capacity) {
		list->m_count = count + 1;
		CAiDestinationEntry* entry = &list->m_entries[count];
		entry->m_type = DESTINATION_OBJECT;
		entry->m_coordinate.m_xFixed = position.m_xFixed;
		entry->m_coordinate.m_yFixed = position.m_yFixed;
		entry->m_coordinate.m_zFixed = position.m_zFixed;
		entry->m_metadata = (unsigned short) p_objectId;
	}
}

// FUNCTION: LEMBALL 0x004147d0
bool CPlayerLemmingGroup::RemoveLemmingFromGroup(CPlayerLemming* p_lemming)
{
	CGenericGroup::RemoveElementFromGroup(p_lemming);
	CPlayerLemming* leader = (CPlayerLemming*) CGenericGroup::GetFirstElementInGroup();
	if (m_playerControlled == 1 && leader != 0) {
		leader->SetGroupLeader(1);
	}
	m_altered = 1;
	return 1;
}

// FUNCTION: LEMBALL 0x00414810
void CPlayerLemmingGroup::SetPlayerControlled(unsigned int p_playerControlled, CPlayerLemming* p_leader)
{
	CPlayerLemming* first = (CPlayerLemming*) CGenericGroup::GetFirstElementInGroup();
	CPlayerLemming* lemming = first;
	if (first != 0) {
		do {
			lemming->SetGroup(p_playerControlled);
			lemming->SetGroupLeader(0);
			lemming = (CPlayerLemming*) CGenericGroup::GetNextElementInGroup();
		} while (lemming != 0);
	}
	if (p_leader == 0) {
		p_leader = first;
	}
	m_playerControlled = p_playerControlled;
	if (p_leader != 0) {
		p_leader->SetGroupLeader(1);
		if (p_leader != first) {
			CGenericGroup::SwapElements(p_leader, first);
		}
	}
}

// FUNCTION: LEMBALL 0x00414880
bool CPlayerLemmingGroup::CheckPlayerControlled()
{
	return m_playerControlled;
}

// FUNCTION: LEMBALL 0x00414890
CPlayerLemming* CPlayerLemmingGroup::GetFirstDeadLemming()
{
	CPlayerLemming* lemming = (CPlayerLemming*) CGenericGroup::GetFirstElementInGroup();
	while (1) {
		if (lemming == 0) {
			return 0;
		}
		if (lemming->m_action == 8) {
			break;
		}
		lemming = (CPlayerLemming*) CGenericGroup::GetNextElementInGroup();
	}
	return lemming;
}

// FUNCTION: LEMBALL 0x004148f0
void CPlayerLemmingGroup::ClearExistingWaypoints()
{
	CGenericGroup::ClearExistingWaypoints();
	if (m_useObject != 0) {
		if (GetGroupState() == GROUP_STATE_USING_OBJECT && m_useObject->m_heading != 0 &&
			m_useObject->m_activationReserved != 0) {
			if (g_pActiveConnection != 0) {
				m_useObject->SendCancel();
			}
			m_useObject->m_activationReserved = 0;
		}
		m_currentUseElement = GetElementsInGroup();
	}
}

// FUNCTION: LEMBALL 0x004149a0
bool CPlayerLemmingGroup::HasSFXChanged()
{
	bool changed = false;
	CPlayerLemming* lemming = (CPlayerLemming*) CGenericGroup::GetFirstElementInGroup();
	while (lemming != 0) {
		changed = lemming->CheckSFX() || changed;
		lemming = (CPlayerLemming*) CGenericGroup::GetNextElementInGroup();
	}
	return changed;
}
