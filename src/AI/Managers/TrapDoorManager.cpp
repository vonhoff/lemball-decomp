#include "TrapDoorManager.h"

#include "../Objects/TrapDoor.h"
#include "../Objects/ViewData.h"

// 68K 0x1062169e __ct__16CTrapDoorManagerFv
// FUNCTION: LEMBALL 0x0040c750
TrapDoorManager::TrapDoorManager() : BaseObjectManager(0x1e, 0x13)
{
	m_count = 0;
	for (int i = 0; i < 8; i++) {
		m_doors[i] = 0;
	}
}

// 68K 0x1062172c Restart__16CTrapDoorManagerFv
// FUNCTION: LEMBALL 0x0040c7b0
void TrapDoorManager::Restart()
{
	for (int i = 0; i < m_count; i++) {
		m_doors[i]->Restart();
	}
}

// 68K 0x10621816 AddNewDoor__16CTrapDoorManagerFUsR7AICOORDUcUl
// FUNCTION: LEMBALL 0x0040c810
void TrapDoorManager::AddNewDoor(unsigned short p_id,
								 const AiCoord& p_position,
								 unsigned int p_mode,
								 unsigned long p_deadline)
{
	m_doors[m_count] = new TrapDoor((AiCoord&) p_position, p_mode);
	m_doors[m_count]->Restart();
	m_doors[m_count]->SetId(p_id);
	m_doors[m_count]->m_manager = this;
	TrapDoor* door = m_doors[m_count];
	if (p_deadline != 0) {
		door->m_deadline = p_deadline;
	}
	m_count++;
}

// 68K 0x106218d6 GetViewData__16CTrapDoorManagerFP9CViewData
// FUNCTION: LEMBALL 0x0040c890
int TrapDoorManager::GetViewData(ViewData* p_viewData)
{
	int count = 0;
	int i = 0;
	if (m_count > 0) {
		TrapDoor** door = m_doors;
		ViewData* viewData = p_viewData;
		do {
			if ((*door)->m_action != 0x18 && (*door)->m_action != 0x1e) {
				(*door)->GetViewData(*viewData++);
				count++;
			}
			door++;
			i++;
		} while (i < m_count);
	}
	return count;
}

// 68K 0x10621960 Process__16CTrapDoorManagerFv
// FUNCTION: LEMBALL 0x0040c8f0
void TrapDoorManager::Process()
{
	if (m_count != 0) {
		for (int i = 0; i < m_count; i++) {
			m_doors[i]->m_requestEnabled = 1;
			if (m_doors[i]->m_active != 0) {
				if (!m_doors[i]->Process()) {
					m_doors[i]->m_active = 0;
				}
			}
		}
	}
}

// 68K 0x1062177e __dt__16CTrapDoorManagerFv
TrapDoorManager::~TrapDoorManager()
{
}
