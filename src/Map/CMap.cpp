#include "CMap.h"

#include "CGround.h"
#include "CGroundArray.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Gameplay/Simulation/CAI.h"
#include "Level/LevelFormat.h"

#include <stddef.h>

#define WALK_CELL_SHIFT GROUND_BLOCK_PIXEL_SHIFT
#define WALK_CELL_SIZE GROUND_BLOCK_PIXEL_SIZE
#define WALK_CELL_HALF_SIZE (WALK_CELL_SIZE / 2)
#define WALK_CELL_MASK GROUND_BLOCK_PIXEL_MASK
#define WALK_MAX_HEIGHT_STEP 15
#define WALK_OUT_OF_BOUNDS_COLLISION GROUND_COLLISION_OUT_OF_BOUNDS
#define WALK_BLOCKING_COLLISION_MASK                                                                                   \
	(GROUND_COLLISION_BLOCKS_WALKING | GROUND_COLLISION_HAZARD | GROUND_COLLISION_SPECIAL_RENDER)
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
unsigned short g_wCliffOnlyGroundDataLimit = 0;

// GLOBAL: LEMBALL 0x0049e534
unsigned short g_groundAnimFrameLimit = 0;

// GLOBAL: LEMBALL 0x0049e538
unsigned short g_treeFrameLimit = 0;

// GLOBAL: LEMBALL 0x0049e544
unsigned short g_embersFrameLimit = 1;

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
	m_orientation = MAP_ORIENTATION_ROTATION_0_DEGREES;
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
		int blockX = p_x >> GROUND_BLOCK_PIXEL_SHIFT;
		int blockY = p_y >> GROUND_BLOCK_PIXEL_SHIFT;
		if ((m_ground.m_ground[blockY * m_ground.m_width + blockX].m_collision & GROUND_COLLISION_MOVER_PRESENT) != 0) {
			int height;
			CMover* mover = m_ai->FindMoverHeight(p_x, p_y, height);
			if (mover != NULL) {
				*p_mover = mover;
				return (unsigned short) height;
			}
		}
	}
	int blockX = p_x >> GROUND_BLOCK_PIXEL_SHIFT;
	int blockY = p_y >> GROUND_BLOCK_PIXEL_SHIFT;
	if (p_x >= 0 && p_y >= 0 && blockX < m_ground.m_width && blockY < m_ground.m_height) {
		p_x &= GROUND_BLOCK_PIXEL_MASK;
		p_y &= GROUND_BLOCK_PIXEL_MASK;
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
	int low;
	int coordinate;
	int nextBlock;
	int blockCoordinate;
	int currentBlockY;
	int lowCoordinate;
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
							adjacentBlock = coordinate >> WALK_CELL_SHIFT;
							if ((x < 0) || (coordinate < 0) || m_ground.m_width <= blockCoordinate ||
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
							blockCoordinate = coordinate >> WALK_CELL_SHIFT;
							if ((((coordinate < 0) || (y < 0)) || m_ground.m_width <= blockCoordinate) ||
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
	case MAP_ORIENTATION_ROTATION_0_DEGREES:
		p_gameX = p_screenX / 2 + p_screenY - MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE;
		p_gameY = p_screenY - p_screenX / 2 + MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE;
		break;
	case MAP_ORIENTATION_ROTATION_90_DEGREES:
		p_gameX = p_screenY - p_screenX / 2 + MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE;
		p_gameY = MAP_PROJECTION_BLOCK_PLUS_HALF_PIXEL_SIZE - p_screenX / 2 - p_screenY;
		break;
	case MAP_ORIENTATION_ROTATION_180_DEGREES:
		p_gameX = MAP_PROJECTION_BLOCK_PLUS_HALF_PIXEL_SIZE - p_screenX / 2 - p_screenY;
		p_gameY = p_screenX / 2 - p_screenY + MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE;
		break;
	case MAP_ORIENTATION_ROTATION_270_DEGREES:
		p_gameX = p_screenX / 2 - p_screenY + MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE;
		p_gameY = p_screenX / 2 + p_screenY - MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE;
	}
}

// FUNCTION: LEMBALL 0x00430b30
void CMap::ScreenToGame(int& p_x, int& p_y)
{
	int y;
	int x = p_x;
	y = p_y;
	switch (m_orientation) {
	case MAP_ORIENTATION_ROTATION_0_DEGREES:
		p_x = x / 2 + y - MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE;
		p_y = y - x / 2 + MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE;
		break;
	case MAP_ORIENTATION_ROTATION_90_DEGREES:
		p_x = y - x / 2 + MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE;
		p_y = MAP_PROJECTION_BLOCK_PLUS_HALF_PIXEL_SIZE - x / 2 - y;
		break;
	case MAP_ORIENTATION_ROTATION_180_DEGREES:
		p_x = MAP_PROJECTION_BLOCK_PLUS_HALF_PIXEL_SIZE - x / 2 - y;
		p_y = x / 2 - y + MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE;
		break;
	case MAP_ORIENTATION_ROTATION_270_DEGREES:
		p_x = x / 2 - y + MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE;
		p_y = x / 2 + y - MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE;
	}
}

// FUNCTION: LEMBALL 0x00430be0
void CMap::GameToScreen(int p_gameX, int p_gameY, int& p_screenX, int& p_screenY)
{
	switch (m_orientation) {
	case MAP_ORIENTATION_ROTATION_0_DEGREES:
		p_screenX = p_gameX - p_gameY + MAP_PROJECTION_BLOCK_PIXEL_SIZE;
		p_screenY = p_gameY / 2 + p_gameX / 2;
		break;
	case MAP_ORIENTATION_ROTATION_90_DEGREES:
		p_screenX = MAP_PROJECTION_DOUBLE_BLOCK_PIXEL_SIZE - p_gameY - p_gameX;
		p_screenY = p_gameX / 2 - p_gameY / 2 + MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE;
		break;
	case MAP_ORIENTATION_ROTATION_180_DEGREES:
		p_screenX = p_gameY - p_gameX + MAP_PROJECTION_BLOCK_PIXEL_SIZE;
		p_screenY = MAP_PROJECTION_BLOCK_PIXEL_SIZE - p_gameY / 2 - p_gameX / 2;
		break;
	case MAP_ORIENTATION_ROTATION_270_DEGREES:
		p_screenX = p_gameX + p_gameY;
		p_screenY = p_gameY / 2 - p_gameX / 2 + MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE;
	}
}

// FUNCTION: LEMBALL 0x00430ce0
void CMap::GameToScreen(int& p_x, int& p_y)
{
	int x;
	int* outputY = &p_y;
	int y = *outputY;
	x = p_x;
	switch (m_orientation) {
	case MAP_ORIENTATION_ROTATION_0_DEGREES:
		p_x = x - y + MAP_PROJECTION_BLOCK_PIXEL_SIZE;
		*outputY = y / 2 + x / 2;
		break;
	case MAP_ORIENTATION_ROTATION_90_DEGREES:
		p_x = MAP_PROJECTION_DOUBLE_BLOCK_PIXEL_SIZE - y - x;
		*outputY = x / 2 - y / 2 + MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE;
		break;
	case MAP_ORIENTATION_ROTATION_180_DEGREES:
		p_x = y - x + MAP_PROJECTION_BLOCK_PIXEL_SIZE;
		*outputY = MAP_PROJECTION_BLOCK_PIXEL_SIZE - y / 2 - x / 2;
		break;
	case MAP_ORIENTATION_ROTATION_270_DEGREES:
		p_x = x + y;
		*outputY = y / 2 - x / 2 + MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE;
	}
}

// FUNCTION: LEMBALL 0x00430db0
void CMap::LoadLevel(tagLoadGroundSurfaceData* p_data, unsigned long p_dataSize, unsigned char p_skip)
{
	unsigned short* data;
	int x;
	int y;
	int width = p_data->m_width;
	int height = p_data->m_height;
	data = (unsigned short*) (p_data + 1);

	m_ground.Clear();
	ReSize(width, height);

	for (y = 0; height > y; y++) {
		for (x = 0; width > x; x++) {
			eObjectType objectType = (eObjectType) *data++;
			unsigned short objectData = *data++;
			unsigned short groundHeight = *data++;

			CGround* ground = m_ground.m_ground + m_ground.m_width * y + x;
			ground->m_objectType = objectType;
			ground->m_objectData = objectData;
			ground->SetCollision();

			m_ground.m_ground[m_ground.m_width * y + x].m_height = groundHeight;
		}
	}

	CreateWalkBits();
	CalculateCliff();
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
	case TERRAIN_CLIFF_ONLY_GROUND:
		if (g_wCliffOnlyGroundDataLimit <= *p_data) {
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
	} while (i < MAP_LEVEL_NAME_MAX_CHARACTERS);
	m_levelName[MAP_LEVEL_NAME_MAX_CHARACTERS] = '\0';
}

// FUNCTION: LEMBALL 0x00431030
void CMap::CalculateCliff()
{
	int x;
	int width;
	CGround* ground;
	int y;
	y = 0;
	if (m_walkHeight > 0) {
		do {
			x = 0;
			for (;;) {
				width = m_walkWidth;
				if (x >= width) {
					break;
				}
				ground = m_ground.m_ground + m_ground.m_width * y + x;
				int height = ground->m_height;
				if (x < width - 1 && y < m_walkHeight - 1) {
					CGround* below = m_ground.m_ground + (y + 1) * m_ground.m_width + x;
					CGround* right = ground + 1;
					int lowerHeight;
					if (right->m_height > below->m_height) {
						lowerHeight = below->m_height;
					}
					else {
						lowerHeight = right->m_height;
					}
					height -= lowerHeight;
				}
				ground->m_cliff = (unsigned short) ((height + GROUND_BLOCK_PIXEL_MASK) / GROUND_BLOCK_PIXEL_SIZE);
				x++;
			}
			y++;
		} while (y < m_walkHeight);
	}
}
