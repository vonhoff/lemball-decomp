#include "CollectableManager.h"

#include "../Navigation/Ai.h"
#include "../Objects/Bonus.h"
#include "../Objects/Collectable.h"
#include "../Objects/Flag.h"
#include "../Objects/TimeBonus.h"
#include "../Objects/ViewData.h"

// 68K 0x10605528 __ct__19CCollectableManagerFP3CAIi
// FUNCTION: LEMBALL 0x004223c0
CollectableManager::CollectableManager(Ai* p_arg0, int p_arg1) : BaseObjectManager(0x11, 6)
{
	m_ai = p_arg0;
	m_count = 0;
	m_capacity = p_arg1;
	m_collectables = 0;
}

// 68K 0x106055be Restart__19CCollectableManagerFv
// FUNCTION: LEMBALL 0x00422420
void CollectableManager::Restart()
{
	if (m_collectables != 0) {
		for (int i = 0; i < m_capacity; i++) {
			if (m_collectables[i] != 0) {
				m_collectables[i]->Restart();
			}
		}
	}
}

// 68K 0x10605628 Initialise__19CCollectableManagerFi
// FUNCTION: LEMBALL 0x00422460
void CollectableManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	if (p_capacity == 0) {
		m_collectables = 0;
		return;
	}
	int i = 0;
	if (m_collectables == 0) {
		m_collectables = new Collectable*[p_capacity];
		for (; i < m_capacity; i++) {
			m_collectables[i] = 0;
		}
	}
}

// 68K 0x10605754 Process__19CCollectableManagerFv
// FUNCTION: LEMBALL 0x00422550
void CollectableManager::Process()
{
	for (int i = 0; i < m_count; i++) {
		m_collectables[i]->m_requestEnabled = 1;
		m_collectables[i]->Process();
	}
}

// 68K 0x106057bc Add__19CCollectableManagerFUsiii11eObjectType
// FUNCTION: LEMBALL 0x00422590
void CollectableManager::Add(unsigned short p_id, int p_x, int p_y, int p_z, eObjectType p_type)
{
	if (m_count < m_capacity) {
		Collectable* collectable;
		switch (p_type) {
		case 0xb:
			collectable = new Flag(p_x, p_y, p_z, (eObjectType) 0xb);
			break;
		case 0xc:
			collectable = new Flag(p_x, p_y, p_z, (eObjectType) 0xc);
			break;
		case 0x12:
			collectable = new Bonus(p_x, p_y, p_z, (eObjectType) 0x12);
			break;
		case 0x1b:
			collectable = new TimeBonus(p_x, p_y, p_z, (eObjectType) 0x1b);
			break;
		}
		collectable->Restart();
		collectable->m_manager = this;
		collectable->SetId(p_id);
		m_collectables[m_count] = collectable;
		m_count++;
	}
}

// 68K 0x10605952 LoadLevel__19CCollectableManagerFPUciUc
// FUNCTION: LEMBALL 0x00422790
void CollectableManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned int p_skip)
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
				id = (unsigned short) GameObject::NextId();
			}
			eObjectType type = (eObjectType) * (unsigned short*) p_data;
			p_data += 2;
			int x = *(unsigned short*) p_data;
			p_data += 2;
			int y = *(unsigned short*) p_data;
			p_data += 2;
			Ai* ai;
			int z = *(unsigned short*) p_data;
			p_data += 2;
			if (p_skip == 0) {
				if (type != (eObjectType) 0xc && type != (eObjectType) 0xb) {
					Add(id, x, y, z, type);
				}
				else {
					ai = m_ai;
					if (ai->m_networkMode != 1) {
						Add(id, x, y, z, type);
					}
					else {
						if (ai->m_isHost == 1) {
							if (type != (eObjectType) 0xc) {
								Add(id, x, y, z, (eObjectType) 0xb);
							}
							else {
								Add(id, x, y, z, (eObjectType) 0xc);
							}
						}
						else if (type == (eObjectType) 0xc) {
							Add(id, x, y, z, (eObjectType) 0xb);
						}
						else {
							Add(id, x, y, z, (eObjectType) 0xc);
						}
					}
				}
			}
			remaining--;
		} while (remaining != 0);
	}
}

// 68K 0x101198ea GetViewData__19CCollectableManagerFP9CViewData
// FUNCTION: LEMBALL 0x00422ef0
int CollectableManager::GetViewData(ViewData* p_viewData)
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

// 68K 0x106056a4 __dt__19CCollectableManagerFv
CollectableManager::~CollectableManager()
{
}
