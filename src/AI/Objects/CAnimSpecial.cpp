#include "CAnimSpecial.h"

#include "../../Map/Base/CMap.h"
#include "../../Visos/Foundation/VsSort.h"
#include "AnimSpecialEntry.h"

enum {
	ANIM_SPECIAL_DIAGONAL_DEPTH_STEP = 64,
	ANIM_SPECIAL_TREE_DEPTH_OFFSET = ANIM_SPECIAL_DIAGONAL_DEPTH_STEP / 2,
	ANIM_SPECIAL_BLOX_1_DEPTH_OFFSET = ANIM_SPECIAL_DIAGONAL_DEPTH_STEP / 4,
	ANIM_SPECIAL_BLOX_2_DEPTH_OFFSET = ANIM_SPECIAL_DIAGONAL_DEPTH_STEP / 8
};

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
					collision = GROUND_COLLISION_OUT_OF_BOUNDS;
				}
				if (collision & GROUND_COLLISION_SPECIAL_RENDER) {
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
		m_entries = NULL;
		return;
	}
	if (m_entries == NULL) {
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
				m_entries[entryIndex].m_sortKey =
					(unsigned short) ((entryRow + entryColumn) * ANIM_SPECIAL_DIAGONAL_DEPTH_STEP);
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
					collision = GROUND_COLLISION_OUT_OF_BOUNDS;
				}
				if (collision & GROUND_COLLISION_SPECIAL_RENDER) {
					int sortOffset = 0;
					switch (objectType) {
					case TERRAIN_TREE:
						sortOffset = ANIM_SPECIAL_TREE_DEPTH_OFFSET;
						break;
					case TERRAIN_BLOX_1:
						sortOffset = ANIM_SPECIAL_BLOX_1_DEPTH_OFFSET;
						break;
					case TERRAIN_BLOX_2:
					case TERRAIN_BLOX_5:
						sortOffset = ANIM_SPECIAL_BLOX_2_DEPTH_OFFSET;
						break;
					}
					m_entries[entryIndex].m_x = (short) entryColumn;
					m_entries[entryIndex].m_y = (short) entryRow;
					m_entries[entryIndex].m_sortKey =
						(unsigned short) ((entryRow + entryColumn) * ANIM_SPECIAL_DIAGONAL_DEPTH_STEP + sortOffset);
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
