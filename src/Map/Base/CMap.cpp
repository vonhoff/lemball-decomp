#include "CMap.h"

#include "../../AI/Navigation/CAI.h"
#include "AI/Base/ObjectTypes.h"
#include "Map/Base/tagLoadDefaultBlox.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"

#include <stddef.h>

#define WALK_CELL_SHIFT 4
#define WALK_CELL_SIZE (1 << WALK_CELL_SHIFT)
#define WALK_CELL_HALF_SIZE (WALK_CELL_SIZE / 2)
#define WALK_CELL_MASK (WALK_CELL_SIZE - 1)
#define WALK_MAX_HEIGHT_STEP 15
#define WALK_OUT_OF_BOUNDS_COLLISION 3
#define WALK_BLOCKING_COLLISION_MASK 0x25
#define WALK_IN_NORTH 0x01
#define WALK_IN_SOUTH 0x02
#define WALK_IN_EAST 0x04
#define WALK_IN_WEST 0x08
#define WALK_OUT_NORTH 0x10
#define WALK_OUT_SOUTH 0x20
#define WALK_OUT_EAST 0x40
#define WALK_OUT_WEST 0x80

// GLOBAL: LEMBALL 0x0049e4e0
CMap* g_pActiveMap = NULL;

// GLOBAL: LEMBALL 0x0049e4e4
CMap* g_pCurrentMap = NULL;

// FUNCTION: LEMBALL 0x004303c0
CMap::CMap()
{
	m_ground.m_ground = NULL;
	m_ground.m_width = 0;
	m_ground.m_height = 0;
	g_pActiveMap = this;
	g_pCurrentMap = this;
	m_walkHeight = 0;
	m_walkWidth = 0;
	m_walkBits = NULL;
}

// FUNCTION: LEMBALL 0x004303f0
void CMap::Restart()
{
	m_orientation = 0;
	m_levelName[0] = '\0';
	m_defaultBlox = TERRAIN_BLOX_4;
	m_defaultBloxData = 0;
	m_ground.Clear();
}

// FUNCTION: LEMBALL 0x00430410
CMap::~CMap()
{
	if (m_walkBits != NULL) {
		delete[] m_walkBits;
	}
	if (m_ground.m_ground != NULL) {
		delete[] m_ground.m_ground;
	}
}

// FUNCTION: LEMBALL 0x00430440
void CMap::ReSize(int p_width, int p_height)
{
	if (m_walkWidth != p_width || m_walkHeight != p_height) {
		m_walkWidth = p_width;
		m_walkHeight = p_height;
		if (m_ground.m_ground != NULL) {
			delete[] m_ground.m_ground;
		}
		m_ground.m_width = p_width;
		m_ground.m_height = p_height;
		m_ground.m_ground = new CGround[p_width * p_height];
		if (m_walkBits != NULL) {
			delete[] m_walkBits;
		}
		m_walkBits = new unsigned char[m_walkWidth * m_walkHeight];
	}
}

// FUNCTION: LEMBALL 0x004304e0
unsigned short CMap::GetZ(int p_x, int p_y, CMover** p_mover)
{
	if (p_mover != NULL) {
		int blockX = p_x >> 4;
		int blockY = p_y >> 4;
		if ((m_ground.m_ground[blockY * m_ground.m_width + blockX].m_collision & 0x10) != 0) {
			int height;
			CMover* mover = m_ai->FindMoverHeight(p_x, p_y, height);
			if (mover != NULL) {
				*p_mover = mover;
				return (unsigned short) height;
			}
		}
	}
	int blockX = p_x >> 4;
	int blockY = p_y >> 4;
	if (p_x >= 0 && p_y >= 0 && blockX < m_ground.m_width && blockY < m_ground.m_height) {
		p_x &= 0xf;
		p_y &= 0xf;
		return m_ground.m_ground[blockY * m_ground.m_width + blockX].GetZ(p_x, p_y);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x004305a0
int CMap::TestWalkBit(int p_x, int p_y, unsigned char p_mask)
{
	if (p_x >= 0 && p_y >= 0 && m_walkWidth > p_x && m_walkHeight > p_y) {
		return (m_walkBits[m_walkWidth * p_y + p_x] & p_mask) == p_mask;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x004305f0
unsigned char CMap::GetWalk(int p_x, int p_y)
{
	if (p_x >= 0 && p_y >= 0 && p_x < m_walkWidth && m_walkHeight > p_y) {
		return m_walkBits[p_y * m_walkWidth + p_x];
	}
	return '\0';
}

// FUNCTION: LEMBALL 0x00430620
void CMap::CreateWalkBits()
{
	int y;
	int x;
	int blockY;
	int blockX;
	unsigned short collision;
	unsigned short z;
	int firstHeight;
	int secondHeight;
	int westHeight;
	int adjacentBlock;
	CGround* ground;
	unsigned int low;
	unsigned int coordinate;
	int nextBlock;
	int blockCoordinate;
	int currentBlockY;
	unsigned int lowCoordinate;
	int lowBlock;
	unsigned char* walkBits;

	blockY = 0;
	walkBits = m_walkBits;
	if (m_walkHeight > 0) {
		y = WALK_CELL_HALF_SIZE;
		do {
			blockX = 0;
			if (m_walkWidth > 0) {
				x = WALK_CELL_HALF_SIZE;
				do {
					*walkBits = 0;

					if (WALK_CELL_HALF_SIZE < y) {
						if ((((x < WALK_CELL_HALF_SIZE) || (adjacentBlock = blockY - 1, adjacentBlock < 0)) ||
							 m_ground.m_width <= blockX) ||
							(m_ground.m_height <= adjacentBlock)) {
							collision = WALK_OUT_OF_BOUNDS_COLLISION;
						}
						else {
							ground = m_ground.GetGroundCell(blockX, adjacentBlock);
							collision = ground->m_collision;
						}
						if ((collision & WALK_BLOCKING_COLLISION_MASK) == 0) {
							collision = m_ground.GetZ(x, y - WALK_CELL_HALF_SIZE);
							firstHeight = collision;
							coordinate = y - (WALK_CELL_HALF_SIZE + 1);
							blockCoordinate = x >> WALK_CELL_SHIFT;
							adjacentBlock = (int) coordinate >> WALK_CELL_SHIFT;
							if ((x < 0) || ((int) coordinate < 0) || m_ground.m_width <= blockCoordinate ||
								m_ground.m_height <= adjacentBlock) {
								z = 0;
							}
							else {
								lowCoordinate = x & WALK_CELL_MASK;
								low = coordinate & WALK_CELL_MASK;
								z = m_ground.GetGroundCell(blockCoordinate, adjacentBlock)->GetZ(lowCoordinate, low);
							}
							secondHeight = z;
							if (secondHeight <= firstHeight + WALK_MAX_HEIGHT_STEP) {
								*walkBits |= WALK_OUT_NORTH;
							}
							if (firstHeight <= secondHeight + WALK_MAX_HEIGHT_STEP) {
								*walkBits |= WALK_IN_NORTH;
							}
						}
					}

					if (blockX < m_walkWidth - 1) {
						nextBlock = blockX + 1;
						if (((nextBlock < 0) || (y < WALK_CELL_HALF_SIZE)) ||
							((m_ground.m_width <= nextBlock) || (m_ground.m_height <= blockY))) {
							collision = WALK_OUT_OF_BOUNDS_COLLISION;
						}
						else {
							ground = m_ground.GetGroundCell(nextBlock, blockY);
							collision = ground->m_collision;
						}
						if ((collision & WALK_BLOCKING_COLLISION_MASK) == 0) {
							coordinate = x + (WALK_CELL_HALF_SIZE - 1);
							currentBlockY = y >> WALK_CELL_SHIFT;
							blockCoordinate = (int) coordinate >> WALK_CELL_SHIFT;
							if (((((int) coordinate < 0) || (y < 0)) || m_ground.m_width <= blockCoordinate) ||
								m_ground.m_height <= currentBlockY) {
								collision = 0;
							}
							else {
								low = coordinate & WALK_CELL_MASK;
								lowCoordinate = y & WALK_CELL_MASK;
								collision =
									m_ground.GetGroundCell(blockCoordinate, currentBlockY)->GetZ(low, lowCoordinate);
							}
							firstHeight = collision;
							nextBlock = (x + WALK_CELL_HALF_SIZE) >> WALK_CELL_SHIFT;
							if (((((x + WALK_CELL_HALF_SIZE) < 0) || (y < 0)) || m_ground.m_width <= nextBlock) ||
								(m_ground.m_height <= currentBlockY)) {
								z = 0;
							}
							else {
								lowCoordinate = y & WALK_CELL_MASK;
								lowBlock = 0;
								z = m_ground.GetGroundCell(nextBlock, currentBlockY)->GetZ(lowBlock, lowCoordinate);
							}
							secondHeight = z;
							if (secondHeight <= firstHeight + WALK_MAX_HEIGHT_STEP) {
								*walkBits |= WALK_OUT_EAST;
							}
							if (firstHeight <= secondHeight + WALK_MAX_HEIGHT_STEP) {
								*walkBits |= WALK_IN_EAST;
							}
						}
					}

					if (blockY < m_walkHeight - 1) {
						if ((((x < WALK_CELL_HALF_SIZE) || (nextBlock = blockY + 1, nextBlock < 0)) ||
							 m_ground.m_width <= blockX) ||
							m_ground.m_height <= nextBlock) {
							collision = WALK_OUT_OF_BOUNDS_COLLISION;
						}
						else {
							ground = m_ground.GetGroundCell(blockX, nextBlock);
							collision = ground->m_collision;
						}
						if ((collision & WALK_BLOCKING_COLLISION_MASK) == 0) {
							blockCoordinate = x >> WALK_CELL_SHIFT;
							nextBlock = (y + (WALK_CELL_HALF_SIZE - 1)) >> WALK_CELL_SHIFT;
							if ((((x < 0) || ((y + (WALK_CELL_HALF_SIZE - 1)) < 0)) ||
								 m_ground.m_width <= blockCoordinate) ||
								m_ground.m_height <= nextBlock) {
								collision = 0;
							}
							else {
								low = x & WALK_CELL_MASK;
								lowCoordinate = (y + (WALK_CELL_HALF_SIZE - 1)) & WALK_CELL_MASK;
								collision =
									m_ground.GetGroundCell(blockCoordinate, nextBlock)->GetZ(low, lowCoordinate);
							}
							firstHeight = collision;
							nextBlock = (y + WALK_CELL_HALF_SIZE) >> WALK_CELL_SHIFT;
							if ((((x < 0) || ((y + WALK_CELL_HALF_SIZE) < 0)) || m_ground.m_width <= blockCoordinate) ||
								m_ground.m_height <= nextBlock) {
								z = 0;
							}
							else {
								lowBlock = 0;
								low = x & WALK_CELL_MASK;
								z = m_ground.GetGroundCell(blockCoordinate, nextBlock)->GetZ(low, lowBlock);
							}
							secondHeight = z;
							if (secondHeight <= firstHeight + WALK_MAX_HEIGHT_STEP) {
								*walkBits |= WALK_OUT_SOUTH;
							}
							if (firstHeight <= secondHeight + WALK_MAX_HEIGHT_STEP) {
								*walkBits |= WALK_IN_SOUTH;
							}
						}
					}

					if (WALK_CELL_HALF_SIZE < x) {
						if (((blockX - 1 < 0) || (y < WALK_CELL_HALF_SIZE)) ||
							((blockCoordinate = m_ground.m_width, blockCoordinate <= blockX - 1) ||
							 m_ground.m_height <= blockY)) {
							collision = WALK_OUT_OF_BOUNDS_COLLISION;
						}
						else {
							collision = m_ground.m_ground[blockCoordinate * blockY + blockX - 1].m_collision;
						}
						if ((collision & WALK_BLOCKING_COLLISION_MASK) == 0) {
							nextBlock = (x - WALK_CELL_HALF_SIZE) >> WALK_CELL_SHIFT;
							currentBlockY = y >> WALK_CELL_SHIFT;
							if (((((x - WALK_CELL_HALF_SIZE) < 0) || (y < 0)) || m_ground.m_width <= nextBlock) ||
								m_ground.m_height <= currentBlockY) {
								collision = 0;
							}
							else {
								lowCoordinate = y & WALK_CELL_MASK;
								lowBlock = 0;
								collision =
									m_ground.GetGroundCell(nextBlock, currentBlockY)->GetZ(lowBlock, lowCoordinate);
							}
							westHeight = collision;
							nextBlock = (x - (WALK_CELL_HALF_SIZE + 1)) >> WALK_CELL_SHIFT;
							if (((((x - (WALK_CELL_HALF_SIZE + 1)) < 0) || (y < 0)) ||
								 ((blockCoordinate = m_ground.m_width, blockCoordinate <= nextBlock) ||
								  m_ground.m_height <= currentBlockY))) {
								z = 0;
							}
							else {
								int cellX = x - (WALK_CELL_HALF_SIZE + 1);
								int cellY = y;
								cellX &= WALK_CELL_MASK;
								cellY &= WALK_CELL_MASK;
								z = (m_ground.m_ground + blockCoordinate * currentBlockY + nextBlock)
										->GetZ(cellX, cellY);
							}
							secondHeight = z;
							if (secondHeight <= westHeight + WALK_MAX_HEIGHT_STEP) {
								*walkBits |= WALK_OUT_WEST;
							}
							if (westHeight <= secondHeight + WALK_MAX_HEIGHT_STEP) {
								*walkBits |= WALK_IN_WEST;
							}
						}
					}

					x += WALK_CELL_SIZE;
					blockX++;
					walkBits++;
				} while (blockX < m_walkWidth);
			}
			y += WALK_CELL_SIZE;
			blockY++;
		} while (blockY < m_walkHeight);
	}
}

// FUNCTION: LEMBALL 0x00430a20
void CMap::SetTerrain(int p_x, int p_y, eObjectType p_objectType, int p_data)
{
	CGround* ground = m_ground.m_ground + m_ground.m_width * p_y + p_x;
	ground->m_objectType = p_objectType;
	ground->m_objectData = p_data;
	ground->SetCollision();
}

// FUNCTION: LEMBALL 0x00430a50
void CMap::ScreenToGame(int p_screenX, int p_screenY, int& p_gameX, int& p_gameY)
{
	switch (m_orientation) {
	case 0:
		p_gameX = p_screenX / 2 + p_screenY - 8;
		p_gameY = p_screenY - p_screenX / 2 + 8;
		break;
	case 1:
		p_gameX = p_screenY - p_screenX / 2 + 8;
		p_gameY = 0x18 - p_screenX / 2 - p_screenY;
		break;
	case 2:
		p_gameX = 0x18 - p_screenX / 2 - p_screenY;
		p_gameY = p_screenX / 2 - p_screenY + 8;
		break;
	case 3:
		p_gameX = p_screenX / 2 - p_screenY + 8;
		p_gameY = p_screenX / 2 + p_screenY - 8;
	}
}

// FUNCTION: LEMBALL 0x00430b30
void CMap::ScreenToGame(int& p_x, int& p_y)
{
	int x = p_x;
	int y = p_y;
	switch (m_orientation) {
	case 0:
		p_x = x / 2 + y - 8;
		p_y = y - x / 2 + 8;
		break;
	case 1:
		p_x = y - x / 2 + 8;
		p_y = 0x18 - x / 2 - y;
		break;
	case 2:
		p_x = 0x18 - x / 2 - y;
		p_y = x / 2 - y + 8;
		break;
	case 3:
		p_x = x / 2 - y + 8;
		p_y = x / 2 + y - 8;
	}
}

// FUNCTION: LEMBALL 0x00430be0
void CMap::GameToScreen(int p_gameX, int p_gameY, int& p_screenX, int& p_screenY)
{
	switch (m_orientation) {
	case 0:
		p_screenX = p_gameX - p_gameY + 0x10;
		p_screenY = p_gameY / 2 + p_gameX / 2;
		break;
	case 1:
		p_screenX = 0x20 - p_gameY - p_gameX;
		p_screenY = p_gameX / 2 - p_gameY / 2 + 8;
		break;
	case 2:
		p_screenX = p_gameY - p_gameX + 0x10;
		p_screenY = 0x10 - p_gameY / 2 - p_gameX / 2;
		break;
	case 3:
		p_screenX = p_gameX + p_gameY;
		p_screenY = p_gameY / 2 - p_gameX / 2 + 8;
	}
}

// FUNCTION: LEMBALL 0x00430e80
void CMap::LoadLevelName(tagLoadGroundName* p_data, unsigned long p_dataSize)
{
	SetLevelName((char*) p_data);
}

// FUNCTION: LEMBALL 0x00430e90
void CMap::LoadDefaultBlox(tagLoadDefaultBlox* p_data, unsigned long p_dataSize)
{
	unsigned int defaultBloxData = p_data->m_objectData;
	unsigned int defaultBlox = p_data->m_objectType;
	m_defaultBlox = (eObjectType) defaultBlox;
	m_defaultBloxData = defaultBloxData;
}

// FUNCTION: LEMBALL 0x00430eb0
bool ValidateDefaultBloxData(eObjectType p_type, unsigned short* p_data)
{
	unsigned short validatedData = *p_data;
	switch (p_type) {
	case TERRAIN_TREE:
		if (*p_data >= g_treeFrameLimit) {
			validatedData = 0;
		}
		break;
	case TERRAIN_BLOX_1:
		if (g_blox1FrameLimit <= *p_data) {
			validatedData = 0;
		}
		break;
	case TERRAIN_BLOX_2:
		if (g_blox2FrameLimit <= *p_data) {
			validatedData = 0;
		}
		break;
	case TERRAIN_BLOX_3_SLOPE_SW_STEEP:
		if (*p_data >= g_steepSwSlopeFrameLimit) {
			validatedData = 0;
		}
		break;
	case TERRAIN_BLOX_4:
		if (*p_data >= g_blox4FrameLimit) {
			validatedData = 0;
		}
		break;
	case TERRAIN_BLOX_5:
		if (*p_data >= g_blox5FrameLimit) {
			validatedData = 0;
		}
		break;
	case TERRAIN_BLOX_6:
		if (*p_data >= g_blox6FrameLimit) {
			validatedData = 0;
		}
		break;
	case TERRAIN_BLOX_7:
		if (g_blox7FrameLimit <= *p_data) {
			validatedData = 0;
		}
		break;
	case TERRAIN_BLOX_8_SLOPE_SE_STEEP:
		if (g_steepSeSlopeFrameLimit <= *p_data) {
			validatedData = 0;
		}
		break;
	case TERRAIN_BLOX_14_SLOPE_SW_SHALLOW:
		if (g_shallowSwSlopeFrameLimit <= *p_data) {
			validatedData = 0;
		}
		break;
	case TERRAIN_BLOX_15_SLOPE_SE_SHALLOW:
		if (g_shallowSeSlopeFrameLimit <= *p_data) {
			validatedData = 0;
		}
		break;
	case TERRAIN_ANIM:
		if (*p_data >= g_groundAnimFrameLimit) {
			validatedData = 0;
		}
		break;
	case TERRAIN_0x214:
		if (g_wDefaultBloxLimit0214 <= *p_data) {
			validatedData = 0;
		}
		break;
	case TERRAIN_FLAME:
		return true;
	case TERRAIN_ELECTRIC:
	case TERRAIN_CONVEYOR_VARIANT_A:
	case TERRAIN_CONVEYOR_VARIANT_B:
		return true;
	case TERRAIN_EMBERS:
		if (g_embersFrameLimit <= *p_data) {
			validatedData = 0;
		}
	}
	if (validatedData != *p_data) {
		*p_data = validatedData;
		return false;
	}
	return true;
}

// FUNCTION: LEMBALL 0x00431010
void CMap::SetLevelName(char* p_name)
{
	int i = 0;
	do {
		char c = *p_name++;
		m_levelName[i] = c;
		if (c == '\0') {
			break;
		}
		i++;
	} while (i < 32);
	m_levelName[32] = '\0';
}

// GLOBAL: LEMBALL 0x004a74b4
CMap* g_pMap;

// GLOBAL: LEMBALL 0x0049e4e8
unsigned short g_blox1FrameLimit = 0;

// GLOBAL: LEMBALL 0x0049e4ec
unsigned short g_blox2FrameLimit = 0;

// GLOBAL: LEMBALL 0x0049e4f0
unsigned short g_steepSwSlopeFrameLimit = 0;

// GLOBAL: LEMBALL 0x0049e4f4
unsigned short g_blox4FrameLimit = 0;

// GLOBAL: LEMBALL 0x0049e4f8
unsigned short g_blox5FrameLimit = 0;

// GLOBAL: LEMBALL 0x0049e4fc
unsigned short g_blox6FrameLimit = 0;

// GLOBAL: LEMBALL 0x0049e500
unsigned short g_blox7FrameLimit = 0;

// GLOBAL: LEMBALL 0x0049e504
unsigned short g_steepSeSlopeFrameLimit = 0;

// GLOBAL: LEMBALL 0x0049e51c
unsigned short g_shallowSwSlopeFrameLimit = 0;

// GLOBAL: LEMBALL 0x0049e520
unsigned short g_shallowSeSlopeFrameLimit = 0;

// GLOBAL: LEMBALL 0x0049e528
unsigned short g_wDefaultBloxLimit0214 = 0;

// GLOBAL: LEMBALL 0x0049e534
unsigned short g_groundAnimFrameLimit = 0;

// GLOBAL: LEMBALL 0x0049e538
unsigned short g_treeFrameLimit = 0;

// GLOBAL: LEMBALL 0x0049e544
unsigned short g_embersFrameLimit = 1;
