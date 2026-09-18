#include "GroundAnim.h"

#include "../../Control/Game/Game.h"
#include "../../Map/Base/Map.h"
#include "AI/Base/Coord3d.h"
#include "AI/Objects/GroundAnimEntry.h"
#include "Map/Ground/Ground.h"
#include "Map/Ground/GroundArray.h"

// 68K 0x1060e30e __ct__11CGroundAnimFv
// FUNCTION: LEMBALL 0x0040cf00
GroundAnim::GroundAnim()
{
}

// 68K 0x1060e332 Restart__11CGroundAnimFv
// FUNCTION: LEMBALL 0x0040cf10
void GroundAnim::Restart()
{
	unsigned int currentTick = g_dwGameTick;
	m_count = 0;
	m_needsValidation = 1;
	m_nextProcessTick = currentTick + 2;
}

// 68K 0x1060e36e __dt__11CGroundAnimFv
// FUNCTION: LEMBALL 0x0040cf30
GroundAnim::~GroundAnim()
{
}

// 68K 0x1060e3ac CheckAllAnims__11CGroundAnimFv
// FUNCTION: LEMBALL 0x0040cf40
bool GroundAnim::CheckAllAnims()
{
	int index = 0;
	while (index < m_count) {
		unsigned int& active = m_entries[index].m_active;
		if (active != 0) {
			switch (m_entries[index].m_mapCell->m_objectType) {
			case 0x210:
				active = 1;
				break;
			case 0x215:
			case 0x216:
				active = 0;
				break;
			case 0x219:
			case 0x21a:
				active = 1;
				break;
			default: {
				int next = index + 1;
				active = 0;
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

// 68K 0x1060e4a4 Process__11CGroundAnimFv
// FUNCTION: LEMBALL 0x0040cff0
void GroundAnim::Process()
{
	if (m_nextProcessTick <= g_dwGameTick) {
		if (m_needsValidation != 0) {
			CheckAllAnims();
			m_needsValidation = 0;
		}

		m_nextProcessTick = g_dwGameTick + 2;
		if (m_count > 0) {
			int index = 0;
			do {
				if (m_entries[index].m_active != 0) {
					switch (m_entries[index].m_direction) {
					case -1:
						m_entries[index].m_currentFrame--;
						if (m_entries[index].m_currentFrame < m_entries[index].m_endFrame) {
							m_entries[index].m_currentFrame = m_entries[index].m_startFrame;
						}
						break;
					case 1:
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

// 68K 0x1060e5ae Check__11CGroundAnimFRC8tCoord3d
// FUNCTION: LEMBALL 0x0040d080
bool GroundAnim::Check(const Coord3d& p_coordinate)
{
	int index = 0;
	if (0 < m_count) {
		GroundAnimEntry* entry = m_entries;
		do {
			if (entry->m_coordinate.m_x == p_coordinate.m_x && entry->m_coordinate.m_y == p_coordinate.m_y) {
				return 1;
			}
			entry++;
			index++;
		} while (index < m_count);
	}
	return 0;
}

// 68K 0x1060e62a Add__11CGroundAnimFRC8tCoord3dUsUs
// FUNCTION: LEMBALL 0x0040d130
void GroundAnim::Add(const Coord3d& p_coordinate, unsigned short p_startFrame, unsigned short p_endFrame)
{
	if (Check(p_coordinate) != 0 || m_count >= 200) {
		return;
	}

	m_entries[m_count].m_active = 1;
	m_entries[m_count].m_coordinate = p_coordinate;
	m_entries[m_count].m_currentFrame = p_startFrame;
	m_entries[m_count].m_startFrame = p_startFrame;
	m_entries[m_count].m_endFrame = p_endFrame;
	m_entries[m_count].m_direction = p_endFrame < p_startFrame ? -1 : 1;

	int blockY = p_coordinate.m_y / 16;
	int blockX = p_coordinate.m_x / 16;
	m_entries[m_count].m_mapCell = &g_pCurrentMap->m_ground.m_ground[blockY * g_pCurrentMap->m_ground.m_width + blockX];

	m_count++;
	for (int i = 0; i < m_count; i++) {
		m_entries[i].m_currentFrame = m_entries[i].m_startFrame;
	}
}

// FUNCTION: LEMBALL 0x0040d230
void GroundAnim::RemoveAtCoordinate(const Coord3d& p_coordinate)
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

// 68K 0x1060e768 LoadLevel__11CGroundAnimFPUciUc
// FUNCTION: LEMBALL 0x0040d2e0
void GroundAnim::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short count = *(unsigned short*) p_data;
	p_data += 2;

	if (count != 0) {
		unsigned int remaining = count;
		do {
			Coord3d coordinate;
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
