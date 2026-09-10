#include "TrapDoorManager.h"

#include "../../Map/Base/Map.h"
#include "../Base/GameObject.h"
#include "../Navigation/Ai.h"
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

// 68K 0x106219d8 LoadLevel__16CTrapDoorManagerFPUciUc
// FUNCTION: LEMBALL 0x0040ca40
void TrapDoorManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned int p_skip)
{
	unsigned short* data = (unsigned short*) p_data;
	int count = *data++;
	int selections[4];
	for (int selection = 0; selection < 4; selection++) {
		selections[selection] = 0;
	}
	int i = 0;
	if (count > 0) {
		do {
			unsigned short id;
			if (p_skip == 0) {
				id = GameObject::NextLoadingId();
			}
			AiCoord position;
			position.m_xFixed = *data++ << 12;
			position.m_yFixed = *data++ << 12;
			data++;
			int y;
			int x;
			y = position.m_yFixed >> 12;
			x = position.m_xFixed >> 12;
			int blockX = x >> 4;
			int blockY = y >> 4;
			Map* map = g_pMap;
			unsigned short z;
			if (x < 0 || y < 0 || blockX >= map->m_ground.m_width || g_pMap->m_ground.m_height <= blockY) {
				z = 0;
			}
			else {
				x &= 0xf;
				y &= 0xf;
				z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(x, y);
			}
			position.m_zFixed = (unsigned int) z << 12;
			selections[i] = *data++;
			if (p_skip == 0) {
				AddNewDoor(id, position, 1, 0);
			}
			g_pAI->AddANetworkStart(position.m_xFixed >> 12, position.m_yFixed >> 12, position.m_zFixed >> 12, i);
			i++;
		} while (i < count);
	}
	g_pAI->SetNetworkTrapDoors(count, selections[0], selections[1], selections[2], selections[3]);
}
