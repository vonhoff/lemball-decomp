#include "CTrapDoorManager.h"

#include "../../Map/Base/CMap.h"
#include "../Base/CGameObject.h"
#include "../Navigation/CAi.h"
#include "../Objects/CTrapDoor.h"
#include "AI/Base/AiCoord.h"
#include "AI/Managers/CBaseObjectManager.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"

// FUNCTION: LEMBALL 0x0040c750
CTrapDoorManager::CTrapDoorManager() : CBaseObjectManager(0x1e, 0x13)
{
	m_count = 0;
	for (int i = 0; i < 8; i++) {
		m_doors[i] = 0;
	}
}

// FUNCTION: LEMBALL 0x0040c7b0
void CTrapDoorManager::Restart()
{
	for (int i = 0; i < m_count; i++) {
		m_doors[i]->Restart();
	}
}

// FUNCTION: LEMBALL 0x0040c7e0
CTrapDoorManager::~CTrapDoorManager()
{
	CTrapDoor** door = m_doors;
	int remaining = 8;
	do {
		delete *door;
		door++;
	} while (--remaining != 0);
}

// FUNCTION: LEMBALL 0x0040c810
void CTrapDoorManager::AddNewDoor(unsigned short p_id,
								  const AiCoord& p_position,
								  unsigned int p_mode,
								  unsigned long p_deadline)
{
	m_doors[m_count] = new CTrapDoor((AiCoord&) p_position, p_mode);
	m_doors[m_count]->Restart();
	m_doors[m_count]->SetId(p_id);
	m_doors[m_count]->m_manager = this;
	CTrapDoor* door = m_doors[m_count];
	if (p_deadline != 0) {
		door->m_deadline = p_deadline;
	}
	m_count++;
}

// FUNCTION: LEMBALL 0x0040c890
int CTrapDoorManager::GetViewData(CViewData* p_viewData)
{
	int count = 0;
	int i = 0;
	if (m_count > 0) {
		CTrapDoor** door = m_doors;
		CViewData* viewData = p_viewData;
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

// FUNCTION: LEMBALL 0x0040c8f0
void CTrapDoorManager::Process()
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

// FUNCTION: LEMBALL 0x0040ca40
void CTrapDoorManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned int p_skip)
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
				id = CGameObject::NextLoadingId();
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
			CMap* map = g_pMap;
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

// FUNCTION: LEMBALL 0x0040cbc0
void CTrapDoorManager::ClearAllTrapDoors()
{
	m_count = 0;
	CTrapDoor** door = m_doors;
	int remaining = 8;
	do {
		if (*door != 0) {
			delete *door;
			*door = 0;
		}
		door++;
	} while (--remaining != 0);
}
