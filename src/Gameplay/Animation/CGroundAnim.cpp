#include "CGroundAnim.h"

#include "Gameplay/Geometry/tCoord3d.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Gameplay/Simulation/GameTime.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Map/CMap.h"

enum eFrameDirection {
	FRAME_DIRECTION_BACKWARD = -1,
	FRAME_DIRECTION_FORWARD = 1
};

// FUNCTION: LEMBALL 0x0040cf00
CGroundAnim::CGroundAnim()
{
}

// FUNCTION: LEMBALL 0x0040cf10
void CGroundAnim::Restart()
{
	unsigned long currentTick = g_dwGameTick;
	m_count = 0;
	m_needsValidation = true;
	m_nextProcessTick = currentTick + GROUND_ANIM_PROCESS_INTERVAL_TICKS;
}

// FUNCTION: LEMBALL 0x0040cf30
CGroundAnim::~CGroundAnim()
{
}

// FUNCTION: LEMBALL 0x0040cf40
bool CGroundAnim::CheckAllAnims()
{
	int next;
	int index = 0;
	while (index < m_count) {
		bool& active = m_entries[index].m_active;
		if (active != 0) {
			switch (m_entries[index].m_mapCell->m_objectType) {
			case TERRAIN_ANIM:
			case TERRAIN_CONVEYOR_VARIANT_A:
			case TERRAIN_CONVEYOR_VARIANT_B:
				active = true;
				break;
			case TERRAIN_FLAME:
			case TERRAIN_ELECTRIC:
				active = false;
				break;
			default: {
				next = index + 1;
				active = false;
				while (next < m_count) {
					m_entries[next - 1] = m_entries[next];
					next++;
				}
				index--;
				m_count--;
				break;
			}
			}
		}
		index++;
	}
	return true;
}

// FUNCTION: LEMBALL 0x0040cff0
void CGroundAnim::Process()
{
	if (m_nextProcessTick <= g_dwGameTick) {
		if (m_needsValidation != 0) {
			CheckAllAnims();
			m_needsValidation = false;
		}

		int index = 0;
		m_nextProcessTick = g_dwGameTick + GROUND_ANIM_PROCESS_INTERVAL_TICKS;
		if (index < m_count) {
			do {
				if (m_entries[index].m_active != 0) {
					switch (m_entries[index].m_direction) {
					case FRAME_DIRECTION_BACKWARD:
						m_entries[index].m_currentFrame--;
						if (m_entries[index].m_currentFrame < m_entries[index].m_endFrame) {
							m_entries[index].m_currentFrame = m_entries[index].m_startFrame;
						}
						break;
					case FRAME_DIRECTION_FORWARD:
						m_entries[index].m_currentFrame++;
						if (m_entries[index].m_endFrame < m_entries[index].m_currentFrame) {
							m_entries[index].m_currentFrame = m_entries[index].m_startFrame;
						}
						break;
					}
					m_entries[index].m_mapCell->m_objectData = m_entries[index].m_currentFrame;
				}
				index++;
			} while (index < m_count);
		}
	}
}

// FUNCTION: LEMBALL 0x0040d080
bool CGroundAnim::Check(const tCoord3d& p_coordinate)
{
	int index = 0;
	if (0 < m_count) {
		GroundAnimEntry* entry = m_entries;
		do {
			if (entry->m_coordinate.m_x == p_coordinate.m_x && entry->m_coordinate.m_y == p_coordinate.m_y) {
				return true;
			}
			entry++;
			index++;
		} while (index < m_count);
	}
	return false;
}

// FUNCTION: LEMBALL 0x0040d0c0
void CGroundAnim::AddStaticGroundAnim(const tCoord3d& p_coordinate)
{
	if (Check(p_coordinate) || m_count >= GROUND_ANIM_ENTRY_CAPACITY) {
		return;
	}

	m_entries[m_count].m_active = false;
	m_entries[m_count].m_direction = 0;
	m_entries[m_count].m_coordinate = p_coordinate;
	++m_count;

	for (int index = 0; index < m_count; ++index) {
		m_entries[index].m_currentFrame = m_entries[index].m_startFrame;
	}
}

// FUNCTION: LEMBALL 0x0040d130
void CGroundAnim::Add(const tCoord3d& p_coordinate, unsigned short p_startFrame, unsigned short p_endFrame)
{
	if (Check(p_coordinate) != 0 || m_count >= GROUND_ANIM_ENTRY_CAPACITY) {
		return;
	}

	m_entries[m_count].m_active = true;
	m_entries[m_count].m_coordinate = p_coordinate;
	m_entries[m_count].m_currentFrame = p_startFrame;
	m_entries[m_count].m_startFrame = p_startFrame;
	m_entries[m_count].m_endFrame = p_endFrame;
	m_entries[m_count].m_direction = p_endFrame < p_startFrame ? FRAME_DIRECTION_BACKWARD : FRAME_DIRECTION_FORWARD;

	int blockY = p_coordinate.m_y / GROUND_BLOCK_PIXEL_SIZE;
	int blockX = p_coordinate.m_x / GROUND_BLOCK_PIXEL_SIZE;
	m_entries[m_count].m_mapCell = &g_pCurrentMap->m_ground.m_ground[blockY * g_pCurrentMap->m_ground.m_width + blockX];

	m_count++;
	for (int i = 0; i < m_count; i++) {
		m_entries[i].m_currentFrame = m_entries[i].m_startFrame;
	}
}

// FUNCTION: LEMBALL 0x0040d230
void CGroundAnim::RemoveAtCoordinate(const tCoord3d& p_coordinate)
{
	for (int i = 0; i < m_count; i++) {
		if (p_coordinate.m_x == m_entries[i].m_coordinate.m_x && p_coordinate.m_y == m_entries[i].m_coordinate.m_y) {
			for (int j = i; j < m_count - 1; j++) {
				m_entries[j] = m_entries[j + 1];
			}
			m_count--;
		}
	}
}

// FUNCTION: LEMBALL 0x0040d2b0
int CGroundAnim::ExportCoordinates(tCoord3d* p_records)
{
	int index = 0;
	if (index < m_count) {
		GroundAnimEntry* entry = m_entries;
		do {
			*p_records++ = entry->m_coordinate;
			entry++;
			index++;
		} while (index < m_count);
	}
	return m_count;
}

// FUNCTION: LEMBALL 0x0040d2e0
void CGroundAnim::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short count = *(unsigned short*) p_data;
	p_data += 2;

	if (count != 0) {
		unsigned int remaining = count;
		do {
			tCoord3d coordinate;
			coordinate.m_x = *(unsigned short*) p_data;
			p_data += 2;
			coordinate.m_y = *(unsigned short*) p_data;
			p_data += 2;
			coordinate.m_z = *(unsigned short*) p_data;
			p_data += 2;
			unsigned int startFrame = *(unsigned short*) p_data;
			p_data += 2;
			unsigned int endFrame = *(unsigned short*) p_data;
			p_data += 2;
			Add(coordinate, startFrame, endFrame);
			remaining--;
		} while (remaining != 0);
	}
}
