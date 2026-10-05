#include "CTrapDoorManager.h"

#include "Map/CMap.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Simulation/CAI.h"
#include "CTrapDoor.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CBaseObjectManager.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"

// FUNCTION: LEMBALL 0x0040c750
CTrapDoorManager::CTrapDoorManager()
	: CBaseObjectManager(NETWORK_OBJECT_MANAGER_MESSAGE_ID_BASE + OBJECT_MANAGER_TRANSPORT_TRAP_DOORS,
						 OBJECT_MANAGER_TRANSPORT_TRAP_DOORS)
{
	m_count = 0;
	for (int i = 0; i < TRAP_DOOR_CAPACITY; i++) {
		m_doors[i] = NULL;
	}
}

// FUNCTION: LEMBALL 0x0040c7b0
void CTrapDoorManager::Restart()
{
	CTrapDoor** door;
	int i = 0;
	if (m_count > 0) {
		door = m_doors;
		do {
			(*door++)->Restart();
			i++;
		} while (i < m_count);
	}
}

// FUNCTION: LEMBALL 0x0040c7e0
CTrapDoorManager::~CTrapDoorManager()
{
	int remaining = TRAP_DOOR_CAPACITY;
	CTrapDoor** door = m_doors;
	do {
		delete *door;
		door++;
	} while (--remaining != 0);
}

// FUNCTION: LEMBALL 0x0040c810
void CTrapDoorManager::AddNewDoor(unsigned short p_id,
								  const AICOORD& p_position,
								  unsigned int p_mode,
								  unsigned long p_deadline)
{
	m_doors[m_count] = new CTrapDoor((AICOORD&) p_position, p_mode);
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
	CTrapDoor** door;
	CViewData* viewData;
	int count = 0;
	int i = 0;
	if (m_count > 0) {
		door = m_doors;
		viewData = p_viewData;
		do {
			if ((*door)->m_action != ACTION_READY && (*door)->m_action != ACTION_DOOR_CLOSED) {
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

// FUNCTION: LEMBALL 0x0040c950
int CTrapDoorManager::GetTrapDoorPosition(AICOORD& p_position, int p_index)
{
	if (m_count == 0) {
		return 0;
	}
	p_position = m_doors[p_index]->m_position;
	int y;
	int x;
	x = p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	y = p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
	int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
	CMap* map = g_pMap;
	unsigned short z;
	if (x < 0 || y < 0 || blockX >= map->m_ground.m_width || g_pMap->m_ground.m_height <= blockY) {
		z = 0;
	}
	else {
		x &= GROUND_BLOCK_PIXEL_MASK;
		y &= GROUND_BLOCK_PIXEL_MASK;
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(x, y);
	}
	p_position.m_zFixed = (unsigned int) z << FIXED_POINT_FRACTION_BITS;
	return 1;
}

// FUNCTION: LEMBALL 0x0040ca10
void CTrapDoorManager::SetTrapDoorPosition(int p_x, int p_y, int p_z, int p_index)
{
	if (p_index < m_count) {
		m_doors[p_index]->SetPositionFromIntegers(p_x, p_y, p_z);
	}
}

// FUNCTION: LEMBALL 0x0040ca40
void CTrapDoorManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned int p_skip)
{
	unsigned char* data = p_data;
	int count = *(unsigned short*) data;
	data += sizeof(unsigned short);
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
			AICOORD position;
			position.m_xFixed = *(unsigned short*) data << FIXED_POINT_FRACTION_BITS;
			data += sizeof(unsigned short);
			position.m_yFixed = *(unsigned short*) data << FIXED_POINT_FRACTION_BITS;
			data += sizeof(unsigned short);
			data += sizeof(unsigned short);
			int y;
			int x;
			y = position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
			x = position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
			int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
			int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
			CMap* map = g_pMap;
			unsigned short z;
			if (x < 0 || y < 0 || blockX >= map->m_ground.m_width || g_pMap->m_ground.m_height <= blockY) {
				z = 0;
			}
			else {
				x &= GROUND_BLOCK_PIXEL_MASK;
				y &= GROUND_BLOCK_PIXEL_MASK;
				z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(x, y);
			}
			position.m_zFixed = (unsigned int) z << FIXED_POINT_FRACTION_BITS;
			selections[i] = *(unsigned short*) data;
			data += sizeof(unsigned short);
			if (p_skip == 0) {
				AddNewDoor(id, position, TRAPDOOR_MODE_NETWORK_START, 0);
			}
			g_pAI->AddANetworkStart(position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
									position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
									position.m_zFixed >> FIXED_POINT_FRACTION_BITS,
									i);
			i++;
		} while (i < count);
	}
	g_pAI->SetNetworkTrapDoors(count, selections[0], selections[1], selections[2], selections[3]);
}

// FUNCTION: LEMBALL 0x0040cbc0
void CTrapDoorManager::ClearAllTrapDoors()
{
	int remaining;
	m_count = 0;
	CTrapDoor** door = m_doors;
	remaining = TRAP_DOOR_CAPACITY;
	do {
		if (*door != NULL) {
			delete *door;
			*door = NULL;
		}
		door++;
	} while (--remaining != 0);
}
