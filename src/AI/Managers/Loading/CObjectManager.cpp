#include "../CObjectManager.h"

#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Navigation/CAI.h"
#include "AI/Objects/CAmmo.h"
#include "AI/Objects/CSwitch.h"

// FUNCTION: LEMBALL 0x0041bbc0
void CObjectManager::LoadLevel(unsigned char* p_data, unsigned long p_length, unsigned int p_append)
{
	int switchIndex = 0;
	unsigned short count = *(unsigned short*) p_data;
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
				unsigned short ammoCount;
				if (m_ai->m_levelVersion >= 8) {
					ammoCount = *(unsigned short*) p_data;
					p_data += 2;
				}
				else {
					ammoCount = 0;
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
