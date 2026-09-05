#include "ObjectManager.h"

#include "../../Visos/Network/Connect.h"
#include "../Base/GameObject.h"
#include "../Messages/GameMessageIds.h"
#include "../Navigation/Ai.h"
#include "../Objects/Ammo.h"
#include "../Objects/Balloon.h"
#include "../Objects/Catapult.h"
#include "../Objects/Crate.h"
#include "../Objects/Duplicator.h"
#include "../Objects/Key.h"
#include "../Objects/Switch.h"
#include "../Objects/Tower.h"
#include "../Objects/ViewData.h"

// 68K 0x1061a098 __ct__14CObjectManagerFP3CAIi
// FUNCTION: LEMBALL 0x0041af60
ObjectManager::ObjectManager(Ai* p_arg0, int p_arg1) : BaseObjectManager(0xc, 1)
{
	g_pObjectManager = this;
	m_ai = p_arg0;
	m_count = 0;
	m_objects = 0;
	m_unk0x38 = 0;
	m_capacity = 0;
	g_wNextSwitchIndex = 0;
}

// 68K 0x1061a136 Restart__14CObjectManagerFv
// FUNCTION: LEMBALL 0x0041afd0
void ObjectManager::Restart()
{
	int removedCount = 0;
	int i;
	if (m_objects != 0 && (i = 0, m_capacity > 0)) {
		do {
			GlobalGameObject* object = m_objects[i];
			if (object != 0) {
				if (object->m_objectType == OBJECT_CRATE) {
					Crate* crate = (Crate*) object;
					GlobalGameObject* contents = crate->m_contents;
					if (contents != 0 && crate->m_contentsType == 0xffff) {
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

// 68K 0x1061a24c Initialise__14CObjectManagerFv
// FUNCTION: LEMBALL 0x0041b0c0
void ObjectManager::Initialise(int p_objectCount)
{
	int capacity = p_objectCount + 4;
	m_capacity = capacity;
	if (m_objects == 0) {
		m_objects = new GlobalGameObject*[(unsigned short) capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_objects[i] = 0;
		}
	}
}

// FUNCTION: LEMBALL 0x0041b110
ObjectManager::~ObjectManager()
{
	int i = 0;
	while (i < m_count) {
		GlobalGameObject* object = m_objects[i];
		if (object != 0) {
			delete object;
		}
		i++;
	}
	delete[] m_objects;
}

// 68K 0x1061a36c AddObject__14CObjectManagerFUsP17CGlobalGameObjectUc
// FUNCTION: LEMBALL 0x0041b2a0
GlobalGameObject* ObjectManager::AddObject(unsigned short p_id, GlobalGameObject* p_object, unsigned char p_active)
{
	if (m_count < m_capacity) {
		m_objects[m_count] = p_object;
		if (p_id != 0xffff) {
			p_object->SetId(p_id);
		}
		m_objects[m_count]->m_heading = 1;
		GlobalGameObject* object = *(GlobalGameObject* volatile*) &m_objects[m_count];
		object->m_unk0x3c = *(unsigned int*) &p_active;
		return m_objects[m_count++];
	}
	return 0;
}

// 68K 0x1061a440 Add__14CObjectManagerFUs7AICOORD11eObjectTypeUs11eObjectType
// FUNCTION: LEMBALL 0x0041b370
GlobalGameObject* ObjectManager::Add(unsigned short p_id,
									 AiCoord p_position,
									 eObjectType p_objectType,
									 unsigned short p_linkedObjectId,
									 eObjectType p_linkedObjectType)
{
	GlobalGameObject* object = 0;
	BaseGlobalObject* linkedObject = 0;
	switch (p_objectType) {
	case OBJECT_CATAPULT:
		object = new Catapult(p_position);
		break;
	case OBJECT_AMMO:
		object = new Ammo(p_position);
		break;
	case OBJECT_TOWER:
		object = new Tower(p_position);
		break;
	case OBJECT_CRATE:
		switch (p_linkedObjectType) {
		case OBJECT_CATAPULT:
			linkedObject = new Catapult(p_position);
			break;
		case 0x15:
		case 0x16:
		case 0x17:
			linkedObject = new Key(p_position, p_linkedObjectType);
			break;
		case 0x27:
		case 0x29:
		case 0x2b:
		case 0x2d:
			linkedObject = new Balloon(p_position, p_linkedObjectType);
			break;
		case 0xffff:
			linkedObject = 0;
			break;
		}
		if (linkedObject != 0) {
			linkedObject->SetId(p_linkedObjectId);
		}
		object = new Crate(p_position, linkedObject, p_linkedObjectId);
		break;
	case OBJECT_SWITCH:
		object = new Switch(p_position, (swMessage) 0, 0, 0, 0);
		break;
	case 0x15:
	case 0x16:
	case 0x17:
		object = new Key(p_position, p_objectType);
		break;
	case OBJECT_DUPLICATOR:
		object = new Duplicator(p_position);
		break;
	case 0x27:
	case 0x29:
	case 0x2b:
	case 0x2d:
		object = new Balloon(p_position, p_objectType);
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

// 68K 0x1061aa4e AddSwitch__14CObjectManagerFUsiii9swMessageiii
// FUNCTION: LEMBALL 0x0041b740
Switch* ObjectManager::AddSwitch(unsigned short p_id,
								 int p_x,
								 int p_y,
								 int p_z,
								 int p_message,
								 int p_legacyFirst,
								 int p_legacyLast,
								 int p_legacyAux)
{
	AiCoord position(p_x << 0xc, p_y << 0xc, p_z << 0xc);
	Switch* object = new Switch(position, (swMessage) p_message, p_legacyFirst, p_legacyLast, p_legacyAux);
	object->Restart();
	return (Switch*) AddObject(p_id, object, 1);
}

// 68K 0x1061ab26 Process__14CObjectManagerFv
// FUNCTION: LEMBALL 0x0041b7d0
void ObjectManager::Process()
{
	int i = 0;
	while (i < m_count) {
		m_objects[i]->m_requestEnabled = 1;
		GlobalGameObject* object = m_objects[i];
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

// 68K 0x1061abea GetViewData__14CObjectManagerFP9CViewData
// FUNCTION: LEMBALL 0x0041b830
int ObjectManager::GetViewData(ViewData* p_viewData)
{
	int count = 0;
	int i = 0;
	while (i < m_count) {
		GlobalGameObject* object = m_objects[i];
		if (object->m_heading != 0 || object->GetSndEffect() != 0) {
			object = m_objects[i];
			if (object->m_objectType != 5 || object->m_action != 0x1b) {
				object->GetViewData(*p_viewData);
				count++;
				p_viewData++;
			}
		}
		i++;
	}
	return count;
}

// 68K 0x1061ace6 FindObject__14CObjectManagerFi
// FUNCTION: LEMBALL 0x0041b8f0
GlobalGameObject* ObjectManager::FindObject(int p_id)
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

// 68K 0x1061ad72 Remove__14CObjectManagerFP17CGlobalGameObject
// FUNCTION: LEMBALL 0x0041b9b0
void ObjectManager::Remove(GlobalGameObject* p_object)
{
	for (int i = 0; i < m_count; i++) {
		if (m_objects[i] == p_object) {
			m_objects[i]->m_heading = 0;
			break;
		}
	}
}

// 68K 0x1061ade4 LoadLevel__14CObjectManagerFPUcUlUc
// FUNCTION: LEMBALL 0x0041bbc0
void ObjectManager::LoadLevel(unsigned char* p_data, unsigned long p_length, unsigned char p_append)
{
	unsigned short count = *(unsigned short*) p_data;
	int switchIndex = 0;
	unsigned short id;
	p_data += 2;
	if (*(unsigned int*) &p_append == 0) {
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
				id = GameObject::NextId();
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
			case 0x15:
			case 0x16:
			case 0x17:
			case OBJECT_TRAP_DOOR:
			case OBJECT_DUPLICATOR:
			case 0x27:
			case 0x29:
			case 0x2b:
			case 0x2d:
				if (*(unsigned int*) &p_append == 0) {
					Add(id, position, objectType, 0xffff, 0xffff);
				}
				break;
			case OBJECT_AMMO: {
				unsigned short ammoCount = 0;
				if (m_ai->m_levelVersion >= 8) {
					ammoCount = *(unsigned short*) p_data;
					p_data += 2;
				}
				if (*(unsigned int*) &p_append == 0) {
					Ammo* ammo = (Ammo*) Add(id, position, objectType, 0xffff, 0xffff);
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
					contentsId = GameObject::NextId();
				}
				if (*(unsigned int*) &p_append == 0) {
					Add(id, position, 0x11, contentsId, contentsType);
				}
				break;
			}
			case OBJECT_SWITCH:
				if (m_ai->m_levelVersion > 1) {
					Switch* object;
					if (*(unsigned int*) &p_append == 0) {
						object = (Switch*) Add(id, position, objectType, 0xffff, 0xffff);
					}
					else {
						GlobalGameObject** objects = m_objects + switchIndex;
						do {
							object = (Switch*) *objects;
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
					if (*(unsigned int*) &p_append == 0) {
						AddSwitch(id, x, y, z, message, legacyFirst, legacyLast, legacyAux);
					}
				}
				break;
			}
			remaining--;
		} while (remaining != 0);
	}
}

// 68K 0x1061b180 ConvertVer0ToVer1__14CObjectManagerFv
// FUNCTION: LEMBALL 0x0041bec0
void ObjectManager::ConvertVer0ToVer1()
{
	unsigned int offset = 0;
	int i = 0;
	while (i < m_count) {
		GlobalGameObject* object = *(GlobalGameObject**) ((unsigned char*) m_objects + offset);
		if (object->m_objectType == OBJECT_SWITCH) {
			object->ConvertVer0ToVer1();
		}
		offset += sizeof(GlobalGameObject*);
		i++;
	}
}

// 68K 0x1061b1fe Receive__14CObjectManagerFUsP17CGlobalGameObjectP15CNetworkMessage
// FUNCTION: LEMBALL 0x0041bf00
bool ObjectManager::Receive(unsigned short p_message, GlobalGameObject* p_object, NetworkMessage* p_networkMessage)
{
	switch (p_message) {
	case MESSAGE_REMOVE_OBJECT:
		Remove(p_object);
		return 1;
	}
	return 0;
}

// GLOBAL: LEMBALL 0x004a74c0
ObjectManager* g_pObjectManager;

// GLOBAL: LEMBALL 0x004a7830
ObjectManager* g_pGenericGroupObjectManager;
