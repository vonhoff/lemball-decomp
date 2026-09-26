#include "CAnimSpecial.h"

#include "../../Map/Base/CMap.h"
#include "../../Visos/Foundation/VsSort.h"
#include "AnimSpecialEntry.h"

// FUNCTION: LEMBALL 0x00409930
void CAnimSpecial::Initialise(CMap* p_map)
{
	CMap* map = p_map;
	int width = map->m_ground.m_width;
	int height = map->m_ground.m_height;
	int entryCount = 0;
	int row;
	int column;

	for (row = 0; row < height; row++) {
		column = 0;
		for (;;) {
			if (column >= width) {
				break;
			}
			CGround* ground = map->m_ground.m_ground + row * map->m_ground.m_width + column;
			switch (ground->m_objectType) {
			case TERRAIN_ANIM:
			case TERRAIN_FLAME:
			case TERRAIN_ELECTRIC:
			case TERRAIN_CONVEYOR_VARIANT_A:
			case TERRAIN_CONVEYOR_VARIANT_B:
				entryCount++;
				break;
			default: {
				unsigned short collision;
				if (column >= 0 && row >= 0 && column < map->m_ground.m_width && row < map->m_ground.m_height) {
					collision = ground->m_collision;
				}
				else {
					collision = 3;
				}
				if (collision & 0x20) {
					entryCount++;
					column++;
					continue;
				}
				break;
			}
			}
			column++;
		}
	}
	m_entryCount = entryCount;
	if (entryCount == 0) {
		m_entries = 0;
		return;
	}
	if (m_entries == 0) {
		m_entries = (AnimSpecialEntry*) operator new(entryCount * sizeof(AnimSpecialEntry));
	}

	int entryIndex = 0;
	int entryColumn;
	int entryRow;
	for (entryRow = 0; entryRow < height; entryRow++) {
		entryColumn = 0;
		for (;;) {
			if (entryColumn >= width) {
				break;
			}
			CGround* ground = map->m_ground.m_ground + entryRow * map->m_ground.m_width + entryColumn;
			eObjectType objectType = ground->m_objectType;
			switch (objectType) {
			case TERRAIN_ANIM:
			case TERRAIN_FLAME:
			case TERRAIN_ELECTRIC:
			case TERRAIN_CONVEYOR_VARIANT_A:
			case TERRAIN_CONVEYOR_VARIANT_B:
				m_entries[entryIndex].m_x = (short) entryColumn;
				m_entries[entryIndex].m_y = (short) entryRow;
				m_entries[entryIndex].m_sortKey = (unsigned short) ((entryRow + entryColumn) * 64);
				m_entries[entryIndex].m_groundEntry =
					map->m_ground.m_ground + map->m_ground.m_width * entryRow + entryColumn;
				entryIndex++;
				break;
			default: {
				unsigned short collision;
				if (entryColumn >= 0 && entryRow >= 0 && entryColumn < map->m_ground.m_width &&
					map->m_ground.m_height > entryRow) {
					collision = ground->m_collision;
				}
				else {
					collision = 3;
				}
				if (collision & 0x20) {
					int sortOffset = 0;
					switch (objectType) {
					case TERRAIN_TREE:
						sortOffset = 32;
						break;
					case TERRAIN_BLOX_1:
						sortOffset = 16;
						break;
					case TERRAIN_BLOX_2:
					case TERRAIN_BLOX_5:
						sortOffset = 8;
						break;
					}
					m_entries[entryIndex].m_x = (short) entryColumn;
					m_entries[entryIndex].m_y = (short) entryRow;
					m_entries[entryIndex].m_sortKey = (unsigned short) ((entryRow + entryColumn) * 64 + sortOffset);
					m_entries[entryIndex].m_groundEntry =
						map->m_ground.m_ground + map->m_ground.m_width * entryRow + entryColumn;
					entryIndex++;
					entryColumn++;
					continue;
				}
				break;
			}
			}
			entryColumn++;
		}
	}
	VSQSort(m_entries, m_entryCount, sizeof(AnimSpecialEntry), AnimSpCmp);
}
