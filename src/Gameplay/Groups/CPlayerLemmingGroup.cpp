#include "CPlayerLemmingGroup.h"

#include "CGenericGroup.h"
#include "Gameplay/Characters/CPlayerLemming.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Geometry/Facing.h"
#include "Gameplay/Navigation/CAiDestinationEntry.h"
#include "Gameplay/Navigation/CAiDestinationList.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Objects/CObjectManager.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/ObjectInteractionStates.h"
#include "Multiplayer/Transport/CConnect.h"

#include <stddef.h>

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
	m_useObject = NULL;
}

// FUNCTION: LEMBALL 0x00414070
CPlayerLemmingGroup::~CPlayerLemmingGroup()
{
}

// FUNCTION: LEMBALL 0x00414080
int CPlayerLemmingGroup::GetViewData(CViewData* p_viewData)
{
	CViewData* view;
	int count = 0;
	CPlayerLemmingGroup* self = this;
	CGameObject* object = self->GetFirstElementInGroup();
	if (object != NULL) {
		view = p_viewData;
		do {
			if (object->m_action != ACTION_WAITING_TO_SPAWN) {
				object->GetViewData(*view);
				view++;
				count++;
			}
			object = self->GetNextElementInGroup();
		} while (object != NULL);
	}
	return count;
}

// FUNCTION: LEMBALL 0x00414130
void CPlayerLemmingGroup::Delete()
{
	if (m_useObject != NULL) {
		m_useObject->m_activationReserved = 0;
	}
	m_useObject = NULL;
}

// FUNCTION: LEMBALL 0x00414150
bool CPlayerLemmingGroup::Process()
{
	int count = 0;
	int moving = 0;
	AICOORD position;
	AICOORD memberPosition;
	int x;
	int y;
	int z;
	CGenericGroup::CalculateBoundingBox(GROUP_BOUNDING_BOX_RADIUS_PIXELS);
	CGameObject* member = CGenericGroup::GetFirstElementInGroup();
	while (member != NULL) {
		count++;
		member->Process();
		if (member->DestinationExists()) {
			moving++;
		}
		member = CGenericGroup::GetNextElementInGroup();
	}
	if (count > 0 && moving == 0) {
		switch (GetGroupState()) {
		case GROUP_STATE_IDLE: {
			CAiDestinationList* list = m_destinationList;
			if (list->m_count > 0) {
				CAiDestinationEntry entry = list->PopFirst();
				switch (entry.m_type) {
				case DESTINATION_COORD:
					SendNewWaypoint(entry.GetCoordinate());
					break;
				case DESTINATION_OBJECT: {
					int id = entry.m_metadata;
					CGameObject* object = g_pGroupObjectManager->FindObject(id);
					if (object != NULL) {
						if (object->m_activationReserved != 0) {
							const AICOORD& activation = object->ActivatePosition();
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
							if (member != NULL) {
								const AICOORD& activation = object->ActivatePosition();
								position.m_xFixed = activation.m_xFixed;
								position.m_yFixed = activation.m_yFixed;
								position.m_zFixed = activation.m_zFixed;
								{
									y = member->m_position.m_yFixed;
									z = member->m_position.m_zFixed;
									x = member->m_position.m_xFixed;
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
		case GROUP_STATE_MOVING:
			member = CGenericGroup::GetNthElementInGroup(m_currentUseElement);
			if (member == NULL) {
				SetGroupState(GROUP_STATE_IDLE);
				m_useObject->m_activationReserved = 0;
				m_useObject = NULL;
			}
			else {
				switch (m_useObject->Usage()) {
				case GROUP_OBJECT_USAGE_GROUP:
					m_currentUseElement++;
					{
						y = member->m_position.m_yFixed;
						z = member->m_position.m_zFixed;
						x = member->m_position.m_xFixed;
						position.m_xFixed = x;
						position.m_yFixed = y;
						position.m_zFixed = z;
					}
					member = CGenericGroup::GetNextElementInGroup();
					while (member != NULL) {
						member->AddDestination(position);
						{
							y = member->m_position.m_yFixed;
							z = member->m_position.m_zFixed;
							x = member->m_position.m_xFixed;
							position.m_xFixed = x;
							position.m_yFixed = y;
							position.m_zFixed = z;
						}
						member = CGenericGroup::GetNextElementInGroup();
					}
					SetGroupState(GROUP_STATE_ATTACKING);
					break;
				case GROUP_OBJECT_USAGE_SINGLE:
					SetGroupState(GROUP_STATE_IDLE);
					m_useObject->m_activationReserved = 0;
					m_useObject = NULL;
					break;
				}
			}
			break;
		case GROUP_STATE_ATTACKING:
			if (m_useObject->m_action == ACTION_READY && moving == 0) {
				if (GetElementsInGroup() <= m_currentUseElement) {
					SetGroupState(GROUP_STATE_IDLE);
					m_useObject->m_activationReserved = 0;
					m_useObject = NULL;
				}
				else {
					member = CGenericGroup::GetNthElementInGroup(m_currentUseElement);
					m_useObject->Activate(member);
					SetGroupState(GROUP_STATE_USING_OBJECT);
				}
			}
			break;
		case GROUP_STATE_USING_OBJECT:
			switch (m_useObject->UsableState()) {
			case GROUP_OBJECT_REQUEST_REJECTED:
				m_useObject->m_activationReserved = 0;
				if (m_useObject->m_objectActive != 0) {
					const AICOORD& activation = m_useObject->ActivatePosition();
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
			case GROUP_OBJECT_REQUEST_ACCEPTED:
				SetGroupState(GROUP_STATE_MOVING);
				break;
			}
			break;
		}
	}
	return true;
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
	CGameObject* p_object = NULL;
	int objectCount = g_wObjectCount;
	for (unsigned int i = 0; (int) i < objectCount; i++) {
		p_object = g_pObjects[(unsigned short) i];
		if (p_object != NULL && (unsigned int) p_object->m_objectId == (unsigned int) p_objectId) {
			break;
		}
	}
	AICOORD position = p_object->ActivatePosition();
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
	AICOORD position = p_object->ActivatePosition();
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
	if (m_playerControlled == 1 && leader != NULL) {
		leader->SetGroupLeader(1);
	}
	m_altered = 1;
	return true;
}

// FUNCTION: LEMBALL 0x00414810
void CPlayerLemmingGroup::SetPlayerControlled(unsigned int p_playerControlled, CPlayerLemming* p_leader)
{
	CPlayerLemming* first = (CPlayerLemming*) CGenericGroup::GetFirstElementInGroup();
	CPlayerLemming* lemming = first;
	if (first != NULL) {
		do {
			lemming->SetGroup(p_playerControlled);
			lemming->SetGroupLeader(0);
			lemming = (CPlayerLemming*) CGenericGroup::GetNextElementInGroup();
		} while (lemming != NULL);
	}
	if (p_leader == NULL) {
		p_leader = first;
	}
	m_playerControlled = p_playerControlled;
	if (p_leader != NULL) {
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
		if (lemming == NULL) {
			return NULL;
		}
		if (lemming->m_action == ACTION_DEAD) {
			break;
		}
		lemming = (CPlayerLemming*) CGenericGroup::GetNextElementInGroup();
	}
	return lemming;
}

// FUNCTION: LEMBALL 0x004148c0
CPlayerLemming* CPlayerLemmingGroup::GetCurrentDeadLemming()
{
	CPlayerLemming* lemming = (CPlayerLemming*) CGenericGroup::GetCurrentElementInGroup();
	while (1) {
		if (lemming == NULL) {
			return NULL;
		}
		if (lemming->m_action == ACTION_DEAD) {
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
	if (m_useObject != NULL) {
		if (GetGroupState() == GROUP_STATE_USING_OBJECT && m_useObject->m_objectActive != 0 &&
			m_useObject->m_activationReserved != 0) {
			if (g_pActiveConnection != NULL) {
				m_useObject->SendCancel();
			}
			m_useObject->m_activationReserved = 0;
		}
		m_currentUseElement = GetElementsInGroup();
	}
}

// FUNCTION: LEMBALL 0x00414960
bool CPlayerLemmingGroup::CheckNetworkStateChanged()
{
	bool changed = false;
	CPlayerLemming* lemming = (CPlayerLemming*) CGenericGroup::GetFirstElementInGroup();
	while (lemming != 0) {
		unsigned int lemmingChanged = lemming->CheckNetworkStateChanged();
		changed = lemmingChanged || changed;
		lemming = (CPlayerLemming*) CGenericGroup::GetNextElementInGroup();
	}
	return changed;
}

// FUNCTION: LEMBALL 0x004149a0
bool CPlayerLemmingGroup::HasSFXChanged()
{
	bool changed = false;
	CPlayerLemming* lemming = (CPlayerLemming*) CGenericGroup::GetFirstElementInGroup();
	while (lemming != NULL) {
		changed = lemming->CheckSFX() || changed;
		lemming = (CPlayerLemming*) CGenericGroup::GetNextElementInGroup();
	}
	return changed;
}
