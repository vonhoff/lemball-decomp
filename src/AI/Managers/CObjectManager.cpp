#include "CObjectManager.h"

#include "../../Visos/Network/CConnect.h"
#include "../Base/CGameObject.h"
#include "../Messages/GameMessageIds.h"
#include "../Navigation/CAi.h"
#include "../Objects/CAmmo.h"
#include "../Objects/CBalloon.h"
#include "../Objects/CCatapult.h"
#include "../Objects/CCrate.h"
#include "../Objects/CDuplicator.h"
#include "../Objects/CKey.h"
#include "../Objects/CSwitch.h"
#include "../Objects/CTower.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CBaseGlobalObject.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Managers/CBaseObjectManager.h"
#include "AI/Objects/SwitchEntry.h"

// FUNCTION: LEMBALL 0x0041af60
CObjectManager::CObjectManager(CAi* p_ai, int p_arg1) : CBaseObjectManager(0xc, 1)
{
	g_pObjectManager = this;
	m_ai = p_ai;
	m_count = 0;
	m_objects = 0;
	m_unk0x38 = 0;
	m_capacity = 0;
	g_wNextSwitchIndex = 0;
}

// FUNCTION: LEMBALL 0x0041afd0
void CObjectManager::Restart()
{
	int removedCount = 0;
	int i;
	if (m_objects != 0 && (i = 0, m_capacity > 0)) {
		do {
			CGlobalGameObject* object = m_objects[i];
			if (object != 0) {
				if (object->m_objectType == OBJECT_CRATE) {
					CCrate* crate = (CCrate*) object;
					CGlobalGameObject* contents = crate->m_contents;
					if (contents != 0 && crate->m_contentsType == OBJECT_INVALID) {
						for (int j = 0; j < m_count; j++) {
							if (m_objects[j] == contents) {
								crate->m_contentsType = contents->m_objectType;
								m_objects[j]->Restart();
								removedCount++;
								m_objects[j]->m_heading = 1;
								m_objects[j] = 0;
								break;
							}
						}
					}
				}
				m_objects[i]->Restart();
				m_objects[i]->m_heading = 1;
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
	if (m_objects == 0) {
		m_objects = new CGlobalGameObject*[(unsigned short) capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_objects[i] = 0;
		}
	}
}

// FUNCTION: LEMBALL 0x0041b110
CObjectManager::~CObjectManager()
{
	int i = 0;
	while (i < m_count) {
		CGlobalGameObject* object = m_objects[i];
		if (object != 0) {
			delete object;
		}
		i++;
	}
	delete[] m_objects;
}

// FUNCTION: LEMBALL 0x0041b2a0
CGlobalGameObject* CObjectManager::AddObject(unsigned short p_id, CGlobalGameObject* p_object, unsigned int p_active)
{
	if (m_count < m_capacity) {
		m_objects[m_count] = p_object;
		if (p_id != 0xffff) {
			p_object->SetId(p_id);
		}
		m_objects[m_count]->m_heading = 1;
		m_objects[m_count]->m_unk0x3c = p_active;
		return m_objects[m_count++];
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041b370
CGlobalGameObject* CObjectManager::Add(unsigned short p_id,
									   AiCoord p_position,
									   eObjectType p_objectType,
									   unsigned short p_linkedObjectId,
									   eObjectType p_linkedObjectType)
{
	CGlobalGameObject* object = 0;
	CBaseGlobalObject* linkedObject = 0;
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
			linkedObject = 0;
			break;
		}
		if (linkedObject != 0) {
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
	if (linkedObject != 0) {
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
	AiCoord position(p_x << 0xc, p_y << 0xc, p_z << 0xc);
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
		if (object->m_heading != 0) {
			object->Process();
			if (g_pActiveConnection != 0) {
				object = m_objects[i];
				if (object->m_heading == 0) {
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
		if (object->m_heading != 0 || object->GetSndEffect() != 0) {
			object = m_objects[i];
			if (object->m_objectType != OBJECT_AMMO || object->m_action != ACTION_0x1b) {
				count++;
				object->GetViewData(*p_viewData++);
			}
		}
		i++;
	}
	return count;
}

// FUNCTION: LEMBALL 0x0041b8f0
CGlobalGameObject* CObjectManager::FindObject(int p_id)
{
	int i = 0;
	while (1) {
		if (m_count <= i) {
			return 0;
		}
		if (m_objects[i]->m_objectId == p_id) {
			break;
		}
		i++;
	}
	if (m_objects[i]->m_heading == 0) {
		return 0;
	}
	return m_objects[i];
}

// FUNCTION: LEMBALL 0x0041b9b0
void CObjectManager::Remove(CGlobalGameObject* p_object)
{
	for (int i = 0; i < m_count; i++) {
		if (m_objects[i] == p_object) {
			m_objects[i]->m_heading = 0;
			break;
		}
	}
}

// FUNCTION: LEMBALL 0x0041bbc0
void CObjectManager::LoadLevel(unsigned char* p_data, unsigned long p_length, unsigned int p_append)
{
	unsigned short count = *(unsigned short*) p_data;
	int switchIndex = 0;
	unsigned short id;
	p_data += 2;
	if (p_append == 0) {
		Initialise(count);
	}
	if (count != 0) {
		unsigned int remaining = count;
		do {
			if (m_ai->m_levelVersion > 1) {
				id = *(unsigned short*) p_data;
				p_data += 2;
			}
			else {
				id = CGameObject::NextId();
			}
			int x = *(unsigned short*) p_data;
			p_data += 2;
			int y = *(unsigned short*) p_data;
			p_data += 2;
			int z = *(unsigned short*) p_data;
			p_data += 2;
			eObjectType objectType = (eObjectType) * (unsigned short*) p_data;
			p_data += 4;
			AiCoord position(x << 0xc, y << 0xc, z << 0xc);
			switch (objectType) {
			case OBJECT_CATAPULT:
			case OBJECT_TOWER:
			case OBJECT_KEY_1:
			case OBJECT_KEY_2:
			case OBJECT_KEY_3:
			case OBJECT_TRAP_DOOR:
			case OBJECT_DUPLICATOR:
			case OBJECT_BALLOON_0:
			case OBJECT_BALLOON_2:
			case OBJECT_BALLOON_4:
			case OBJECT_BALLOON_6:
				if (p_append == 0) {
					Add(id, position, objectType, 0xffff, OBJECT_INVALID);
				}
				break;
			case OBJECT_AMMO: {
				unsigned short ammoCount = 0;
				if (m_ai->m_levelVersion >= 8) {
					ammoCount = *(unsigned short*) p_data;
					p_data += 2;
				}
				if (p_append == 0) {
					CAmmo* ammo = (CAmmo*) Add(id, position, objectType, 0xffff, OBJECT_INVALID);
					ammo->m_ammo = ammoCount;
				}
				break;
			}
			case OBJECT_CRATE: {
				eObjectType contentsType = (eObjectType) * (unsigned short*) p_data;
				p_data += 2;
				unsigned short contentsId;
				if (m_ai->m_levelVersion > 1) {
					contentsId = *(unsigned short*) p_data;
					p_data += 2;
				}
				else {
					contentsId = CGameObject::NextId();
				}
				if (p_append == 0) {
					Add(id, position, OBJECT_CRATE, contentsId, contentsType);
				}
				break;
			}
			case OBJECT_SWITCH:
				if (m_ai->m_levelVersion > 1) {
					CSwitch* object;
					if (p_append == 0) {
						object = (CSwitch*) Add(id, position, objectType, 0xffff, OBJECT_INVALID);
					}
					else {
						CGlobalGameObject** objects = m_objects + switchIndex;
						do {
							object = (CSwitch*) *objects;
							objects++;
							switchIndex++;
							if (object->m_objectType == OBJECT_SWITCH) {
								break;
							}
						} while (switchIndex < m_count);
					}
					object->Load(p_data);
				}
				else {
					int message = *(unsigned short*) p_data;
					p_data += 2;
					unsigned short legacyFirst = *(unsigned short*) p_data;
					p_data += 2;
					unsigned short legacyLast = *(unsigned short*) p_data;
					p_data += 2;
					unsigned short legacyAux = *(unsigned short*) p_data;
					p_data += 2;
					if (p_append == 0) {
						AddSwitch(id, x, y, z, message, legacyFirst, legacyLast, legacyAux);
					}
				}
				break;
			}
			remaining--;
		} while (remaining != 0);
	}
}

// FUNCTION: LEMBALL 0x0041bec0
void CObjectManager::ConvertVer0ToVer1()
{
	for (int i = 0; i < m_count; i++) {
		CGlobalGameObject* object = m_objects[i];
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
		return 1;
	}
	return 0;
}

// GLOBAL: LEMBALL 0x004a74c0
CObjectManager* g_pObjectManager;

// GLOBAL: LEMBALL 0x004a7830
CObjectManager* g_pGenericGroupObjectManager;
