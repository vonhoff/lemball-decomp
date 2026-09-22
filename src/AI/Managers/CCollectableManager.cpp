#include "CCollectableManager.h"

#include "../Navigation/CAi.h"
#include "../Objects/CBonus.h"
#include "../Objects/CCollectable.h"
#include "../Objects/CFlag.h"
#include "../Objects/CTimeBonus.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/ObjectTypes.h"
#include "AI/Managers/CBaseObjectManager.h"

// FUNCTION: LEMBALL 0x004223c0
CCollectableManager::CCollectableManager(CAi* p_ai, int p_capacity) : CBaseObjectManager(0x11, 6)
{
	m_ai = p_ai;
	m_count = 0;
	m_capacity = p_capacity;
	m_collectables = 0;
}

// FUNCTION: LEMBALL 0x00422420
void CCollectableManager::Restart()
{
	if (m_collectables != 0) {
		for (int i = 0; i < m_capacity; i++) {
			CCollectable* collectable = m_collectables[i];
			if (collectable != 0) {
				collectable->Restart();
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00422460
void CCollectableManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	if (p_capacity == 0) {
		m_collectables = 0;
		return;
	}
	int i = 0;
	if (m_collectables == 0) {
		m_collectables = new CCollectable*[p_capacity];
		for (; i < m_capacity; i++) {
			m_collectables[i] = 0;
		}
	}
}

// FUNCTION: LEMBALL 0x004224c0
CCollectableManager::~CCollectableManager()
{
	if (m_collectables != 0) {
		for (int i = 0; i < m_count; i++) {
			delete m_collectables[i];
		}
		operator delete(m_collectables);
	}
}

// FUNCTION: LEMBALL 0x00422550
void CCollectableManager::Process()
{
	for (int i = 0; i < m_count; i++) {
		m_collectables[i]->m_requestEnabled = 1;
		m_collectables[i]->Process();
	}
}

// FUNCTION: LEMBALL 0x00422590
void CCollectableManager::Add(unsigned short p_id, int p_x, int p_y, int p_z, eObjectType p_type)
{
	if (m_count < m_capacity) {
		CCollectable* collectable;
		switch (p_type) {
		case OBJECT_FLAG_1:
			collectable = new CFlag(p_x, p_y, p_z, OBJECT_FLAG_1);
			break;
		case OBJECT_FLAG_2:
			collectable = new CFlag(p_x, p_y, p_z, OBJECT_FLAG_2);
			break;
		case OBJECT_BONUS:
			collectable = new CBonus(p_x, p_y, p_z, OBJECT_BONUS);
			break;
		case OBJECT_TIME_BONUS:
			collectable = new CTimeBonus(p_x, p_y, p_z, OBJECT_TIME_BONUS);
			break;
		}
		collectable->Restart();
		collectable->m_manager = this;
		collectable->SetId(p_id);
		m_collectables[m_count] = collectable;
		m_count++;
	}
}

// FUNCTION: LEMBALL 0x00422710
void CCollectableManager::RemoveCollectable(CGameObject* p_object)
{
	for (int i = 0; i < m_count; i++) {
		if (m_collectables[i] == p_object) {
			p_object->Delete();
			p_object->SetId(0xffff);
			delete p_object;
			m_count--;
			for (; i < m_count; i++) {
				m_collectables[i] = m_collectables[i + 1];
			}
			return;
		}
	}
}

// FUNCTION: LEMBALL 0x00422790
void CCollectableManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned int p_skip)
{
	unsigned short count = *(unsigned short*) p_data;
	p_data += 2;
	if (p_skip == 0) {
		Initialise(count);
	}
	if (count != 0) {
		unsigned int remaining = count;
		do {
			unsigned short id;
			if (m_ai->m_levelVersion > 1) {
				id = *(unsigned short*) p_data;
				p_data += 2;
			}
			else {
				id = (unsigned short) CGameObject::NextId();
			}
			eObjectType type = (eObjectType) * (unsigned short*) p_data;
			p_data += 2;
			int x = *(unsigned short*) p_data;
			p_data += 2;
			int y = *(unsigned short*) p_data;
			p_data += 2;
			CAi* ai;
			int z = *(unsigned short*) p_data;
			p_data += 2;
			if (p_skip == 0) {
				if (type != OBJECT_FLAG_2 && type != OBJECT_FLAG_1) {
					Add(id, x, y, z, type);
				}
				else {
					ai = m_ai;
					if (ai->m_networkMode != 1) {
						Add(id, x, y, z, type);
					}
					else {
						if (ai->m_isHost == 1) {
							if (type != OBJECT_FLAG_2) {
								Add(id, x, y, z, OBJECT_FLAG_1);
							}
							else {
								Add(id, x, y, z, OBJECT_FLAG_2);
							}
						}
						else if (type == OBJECT_FLAG_2) {
							Add(id, x, y, z, OBJECT_FLAG_1);
						}
						else {
							Add(id, x, y, z, OBJECT_FLAG_2);
						}
					}
				}
			}
			remaining--;
		} while (remaining != 0);
	}
}

// FUNCTION: LEMBALL 0x00422ef0
int CCollectableManager::GetViewData(CViewData* p_viewData)
{
	int count = 0;
	for (int i = 0; i < m_count; i++) {
		if (m_collectables[i]->m_enabled != 0 || m_collectables[i]->m_soundEffect != 0) {
			m_collectables[i]->GetViewData(*p_viewData++);
			count++;
		}
	}
	return count;
}
