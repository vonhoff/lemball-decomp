#include "CollectableManager.h"

#include "../Objects/Collectable.h"
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
// STUB: LEMBALL 0x00422590
void CollectableManager::Add(unsigned short p_id, int p_x, int p_y, int p_z, eObjectType p_type)
{
}

// 68K 0x10605952 LoadLevel__19CCollectableManagerFPUciUc
// STUB: LEMBALL 0x00422790
void CollectableManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
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
