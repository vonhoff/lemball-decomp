#include "CObjectManager.h"

#include "Visos/Network/CConnect.h"
#include "CGameObject.h"
#include "Gameplay/Messages/GameMessageIds.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Collectables/CAmmo.h"
#include "Gameplay/Mechanisms/CBalloon.h"
#include "Gameplay/Mechanisms/CCatapult.h"
#include "Gameplay/Mechanisms/CCrate.h"
#include "Gameplay/Mechanisms/CDuplicator.h"
#include "Gameplay/Collectables/CKey.h"
#include "Gameplay/Mechanisms/CSwitch.h"
#include "Gameplay/Mechanisms/CTower.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "CBaseGlobalObject.h"
#include "CGlobalGameObject.h"
#include "Gameplay/Geometry/Rect.h"
#include "CBaseObjectManager.h"
#include "Gameplay/Mechanisms/SwitchEntry.h"
#include "Visos/Math/FixedPoint.h"

// FUNCTION: LEMBALL 0x0041af60
CObjectManager::CObjectManager(CAI* p_ai, int p_arg1)
	: CBaseObjectManager(NETWORK_OBJECT_MANAGER_MESSAGE_ID_BASE + OBJECT_MANAGER_TRANSPORT_OBJECTS,
						 OBJECT_MANAGER_TRANSPORT_OBJECTS)
{
	g_pObjectManager = this;
	m_ai = p_ai;
	m_count = 0;
	m_objects = NULL;
	m_unk0x38 = 0;
	m_capacity = 0;
	g_wNextSwitchIndex = 0;
}

// FUNCTION: LEMBALL 0x0041afd0
void CObjectManager::Restart()
{
	int removedCount = 0;
	int i;
	if (m_objects != NULL && (i = 0, m_capacity > 0)) {
		do {
			CGlobalGameObject* object = m_objects[i];
			if (object != NULL) {
				if (object->m_objectType == OBJECT_CRATE) {
					CCrate* crate = (CCrate*) object;
					CGlobalGameObject* contents = crate->m_contents;
					if (contents != NULL && crate->m_contentsType == OBJECT_INVALID) {
						for (int j = 0; j < m_count; j++) {
							if (m_objects[j] == contents) {
								crate->m_contentsType = contents->m_objectType;
								m_objects[j]->Restart();
								removedCount++;
								m_objects[j]->m_objectActive = 1;
								m_objects[j] = NULL;
								break;
							}
						}
					}
				}
				m_objects[i]->Restart();
				m_objects[i]->m_objectActive = 1;
			}
			i++;
		} while (i < m_capacity);
	}
	m_count = m_count - removedCount;
}

// FUNCTION: LEMBALL 0x0041b0c0
void CObjectManager::Initialise(int p_objectCount)
{
	int capacity = p_objectCount + 4;
	m_capacity = capacity;
	if (m_objects == NULL) {
		m_objects = new CGlobalGameObject*[(unsigned short) capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_objects[i] = NULL;
		}
	}
}

// FUNCTION: LEMBALL 0x0041b110
CObjectManager::~CObjectManager()
{
	int i = 0;
	while (i < m_count) {
		CGlobalGameObject* object = m_objects[i];
		if (object != NULL) {
			delete object;
		}
		i++;
	}
	delete[] m_objects;
}

// FUNCTION: LEMBALL 0x0041b160
void CObjectManager::ClearAllObjects()
{
	for (int index = 0; index < m_count; index++) {
		m_objects[index]->SetId(INVALID_OBJECT_ID);
		delete m_objects[index];
	}
	m_count = 0;
	m_unk0x38 = 0;
}

// FUNCTION: LEMBALL 0x0041b1b0
void CObjectManager::DeleteObjectAndLinkedTargets(CGlobalGameObject* p_object)
{
	int index = 0;
	int linkedIndex;
	for (; index < m_count; index++) {
		if (m_objects[index] == p_object) {
			p_object->Delete();
			p_object->SetId(INVALID_OBJECT_ID);
			delete p_object;
			index++;
			while (index < m_count) {
				m_objects[index - 1] = m_objects[index];
				index++;
			}
			m_count--;
			return;
		}
		linkedIndex = 0;
		while (1) {
			if (linkedIndex >= m_count) {
				break;
			}
			if (m_objects[linkedIndex]->m_objectType == OBJECT_CRATE) {
				unsigned short contentsId = ((CCrate*) m_objects[linkedIndex])->m_contentsId;
				if ((unsigned short) p_object->GetId() == contentsId) {
					DeleteObjectAndLinkedTargets(m_objects[linkedIndex]);
				}
			}
			linkedIndex++;
		}
	}
}

// FUNCTION: LEMBALL 0x0041b2a0
CGlobalGameObject* CObjectManager::AddObject(unsigned short p_id, CGlobalGameObject* p_object, unsigned int p_active)
{
	if (m_count < m_capacity) {
		m_objects[m_count] = p_object;
		if (p_id != INVALID_OBJECT_ID) {
			p_object->SetId(p_id);
		}
		m_objects[m_count]->m_objectActive = 1;
		m_objects[m_count]->m_initiallyActive = p_active;
		return m_objects[m_count++];
	}
	return NULL;
}

// FUNCTION: LEMBALL 0x0041b310
CGlobalGameObject* CObjectManager::Add(unsigned short p_id,
									   int p_x,
									   int p_y,
									   int p_z,
									   eObjectType p_objectType,
									   unsigned short p_linkedObjectId,
									   eObjectType p_linkedObjectType)
{
	AICOORD position(p_x << FIXED_POINT_FRACTION_BITS,
					 p_y << FIXED_POINT_FRACTION_BITS,
					 p_z << FIXED_POINT_FRACTION_BITS);
	return Add(p_id, position, p_objectType, p_linkedObjectId, p_linkedObjectType);
}

// FUNCTION: LEMBALL 0x0041b370
CGlobalGameObject* CObjectManager::Add(unsigned short p_id,
									   AICOORD p_position,
									   eObjectType p_objectType,
									   unsigned short p_linkedObjectId,
									   eObjectType p_linkedObjectType)
{
	CGlobalGameObject* object = NULL;
	CBaseGlobalObject* linkedObject = NULL;
	switch (p_objectType) {
	case OBJECT_CATAPULT:
		object = new CCatapult(p_position);
		break;
	case OBJECT_AMMO:
		object = new CAmmo(p_position);
		break;
	case OBJECT_TOWER:
		object = new CTower(p_position);
		break;
	case OBJECT_CRATE:
		switch (p_linkedObjectType) {
		case OBJECT_CATAPULT:
			linkedObject = new CCatapult(p_position);
			break;
		case OBJECT_KEY_1:
		case OBJECT_KEY_2:
		case OBJECT_KEY_3:
			linkedObject = new CKey(p_position, p_linkedObjectType);
			break;
		case OBJECT_BALLOON_0:
		case OBJECT_BALLOON_2:
		case OBJECT_BALLOON_4:
		case OBJECT_BALLOON_6:
			object = new CBalloon(p_position, p_linkedObjectType);
			break;
		case OBJECT_INVALID:
			linkedObject = NULL;
			break;
		}
		if (linkedObject != NULL) {
			linkedObject->SetId(p_linkedObjectId);
		}
		object = new CCrate(p_position, linkedObject, p_linkedObjectId);
		break;
	case OBJECT_SWITCH:
		object = new CSwitch(p_position, SW_NONE, 0, 0, 0);
		break;
	case OBJECT_KEY_1:
	case OBJECT_KEY_2:
	case OBJECT_KEY_3:
		object = new CKey(p_position, p_objectType);
		break;
	case OBJECT_DUPLICATOR:
		object = new CDuplicator(p_position);
		break;
	case OBJECT_BALLOON_0:
	case OBJECT_BALLOON_2:
	case OBJECT_BALLOON_4:
	case OBJECT_BALLOON_6:
		object = new CBalloon(p_position, p_objectType);
		break;
	}
	object->m_manager = this;
	object->Restart();
	if (linkedObject != NULL) {
		linkedObject->m_manager = this;
		linkedObject->Restart();
	}
	return AddObject(p_id, object, 1);
}

// FUNCTION: LEMBALL 0x0041b740
CSwitch* CObjectManager::AddSwitch(unsigned short p_id,
								   int p_x,
								   int p_y,
								   int p_z,
								   int p_message,
								   int p_legacyFirst,
								   int p_legacyLast,
								   int p_legacyAux)
{
	AICOORD position(p_x << FIXED_POINT_FRACTION_BITS,
					 p_y << FIXED_POINT_FRACTION_BITS,
					 p_z << FIXED_POINT_FRACTION_BITS);
	CSwitch* object = new CSwitch(position, (swMessage) p_message, p_legacyFirst, p_legacyLast, p_legacyAux);
	object->Restart();
	return (CSwitch*) AddObject(p_id, object, 1);
}

// FUNCTION: LEMBALL 0x0041b7d0
void CObjectManager::Process()
{
	int i = 0;
	while (i < m_count) {
		m_objects[i]->m_requestEnabled = 1;
		CGlobalGameObject* object = m_objects[i];
		if (object->m_objectActive != 0) {
			object->Process();
			if (g_pActiveConnection != NULL) {
				object = m_objects[i];
				if (object->m_objectActive == 0) {
					object->SendRemove();
				}
			}
		}
		i++;
	}
}

// FUNCTION: LEMBALL 0x0041b830
int CObjectManager::GetViewData(CViewData* p_viewData)
{
	int i = 0;
	int count = i;
	while (i < m_count) {
		CGlobalGameObject* object = m_objects[i];
		if (object->m_objectActive != 0 || object->GetSndEffect() != 0) {
			object = m_objects[i];
			if (object->m_objectType != OBJECT_AMMO || object->m_action != ACTION_RUNNING) {
				count++;
				object->GetViewData(*p_viewData++);
			}
		}
		i++;
	}
	return count;
}

// FUNCTION: LEMBALL 0x0041b8a0
void CObjectManager::ActivateObjectsById(int p_id, CGameObject* p_activator)
{
	for (int index = 0; index < m_count; index++) {
		if (m_objects[index]->m_objectId == p_id) {
			m_objects[index]->Activate(p_activator);
		}
	}
}

// FUNCTION: LEMBALL 0x0041b8f0
CGlobalGameObject* CObjectManager::FindObject(int p_id)
{
	int i = 0;
	while (1) {
		if (m_count <= i) {
			return NULL;
		}
		if (m_objects[i]->m_objectId == p_id) {
			break;
		}
		i++;
	}
	if (m_objects[i]->m_objectActive == 0) {
		return NULL;
	}
	return m_objects[i];
}

// FUNCTION: LEMBALL 0x0041b940
void CObjectManager::RemoveById(short p_id)
{
	for (int index = 0; index < m_count; index++) {
		CGlobalGameObject* object = m_objects[index];
		if (object != NULL && object->GetId() == p_id) {
			DeactivateObjectAtIndex(index);
			return;
		}
	}
}

// FUNCTION: LEMBALL 0x0041b990
void CObjectManager::DeactivateObjectAtIndex(int p_index)
{
	m_objects[p_index]->m_objectActive = 0;
}

// FUNCTION: LEMBALL 0x0041b9b0
void CObjectManager::Remove(CGlobalGameObject* p_object)
{
	for (int i = 0; i < m_count; i++) {
		if (m_objects[i] == p_object) {
			m_objects[i]->m_objectActive = 0;
			break;
		}
	}
}

// FUNCTION: LEMBALL 0x0041b9f0
CGlobalGameObject* CObjectManager::FindNearbyObject(AICOORD p_position)
{
	int x = p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int y = p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int index = 0;
	Rect bounds;
	if (m_count != 0) {
		CGlobalGameObject** objects = m_objects;
		do {
			bounds.m_left = ((*objects)->m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) - 8;
			bounds.m_top = ((*objects)->m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) - 8;
			bounds.m_right = bounds.m_left + 8;
			bounds.m_bottom = bounds.m_top + 8;
			if (x > bounds.m_left && x < bounds.m_right && y > bounds.m_top && y < bounds.m_bottom) {
				return m_objects[index];
			}
			objects++;
			index++;
		} while (index < m_count);
	}
	return NULL;
}

// FUNCTION: LEMBALL 0x0041ba80
CGlobalGameObject* CObjectManager::FindNearbyObject(AICOORD p_position, eObjectType p_objectType)
{
	int x = p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int y = p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int count = m_count;
	int index = 0;
	while (1) {
		if (index >= count) {
			return NULL;
		}
		CGlobalGameObject* object = m_objects[index];
		if (object->m_objectType == p_objectType) {
			const int& left = (object->m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) - 8;
			const int& top = (object->m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) - 8;
			int right = left + 8;
			int bottom = top + 8;
			if (left < x && x < right && y > top && y < bottom) {
				break;
			}
		}
		index++;
	}
	return m_objects[index];
}

// FUNCTION: LEMBALL 0x0041bb10
CGlobalGameObject* CObjectManager::FindObjectInBounds(CVSRect* p_bounds, eObjectType p_objectType)
{
	Rect query;
	query.m_left = p_bounds->m_x;
	query.m_top = p_bounds->m_y;
	query.m_right = query.m_left + p_bounds->m_width;
	query.m_bottom = query.m_top + p_bounds->m_height;
	int index = 0;
	int count = m_count;
	Rect bounds;
	while (1) {
		if (index >= count) {
			return NULL;
		}
		CGlobalGameObject* object = m_objects[index];
		if (object->m_objectType == p_objectType) {
			bounds.m_left = (object->m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) - 8;
			bounds.m_top = (object->m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) - 8;
			bounds.m_right = bounds.m_left + 8;
			bounds.m_bottom = bounds.m_top + 8;
			if (bounds.m_left < query.m_right && query.m_left < bounds.m_right && bounds.m_top < query.m_bottom &&
				query.m_top < bounds.m_bottom) {
				break;
			}
		}
		index++;
	}
	return m_objects[index];
}

// FUNCTION: LEMBALL 0x0041bec0
void CObjectManager::ConvertVer0ToVer1()
{
	for (int objectIndex = 0; objectIndex < m_count; objectIndex++) {
		CGlobalGameObject* object = m_objects[objectIndex];
		if (object->m_objectType == OBJECT_SWITCH) {
			object->ConvertVer0ToVer1();
		}
	}
}

// FUNCTION: LEMBALL 0x0041bf00
bool CObjectManager::Receive(unsigned short p_message, CGlobalGameObject* p_object, CNetworkMessage* p_networkMessage)
{
	switch (p_message) {
	case MESSAGE_REMOVE_OBJECT:
		Remove(p_object);
		return true;
	}
	return false;
}

// GLOBAL: LEMBALL 0x004a74c0
CObjectManager* g_pObjectManager;

// GLOBAL: LEMBALL 0x004a7830
CObjectManager* g_pGenericGroupObjectManager;
