#include "CMap.h"

#include "../../AI/Navigation/CAI.h"
#include "AI/Base/ObjectTypes.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"

// GLOBAL: LEMBALL 0x0049e4e0
CMap* g_pActiveMap = 0;

// GLOBAL: LEMBALL 0x0049e4e4
CMap* g_pCurrentMap = 0;

// FUNCTION: LEMBALL 0x004303c0
CMap::CMap()
{
	m_ground.m_ground = 0;
	m_ground.m_width = 0;
	m_ground.m_height = 0;
	g_pActiveMap = this;
	g_pCurrentMap = this;
	m_walkHeight = 0;
	m_walkWidth = 0;
	m_walkBits = 0;
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
	if (m_walkBits != 0) {
		delete[] m_walkBits;
	}
	if (m_ground.m_ground != 0) {
		delete[] m_ground.m_ground;
	}
}

// FUNCTION: LEMBALL 0x00430440
void CMap::ReSize(int p_width, int p_height)
{
	if (m_walkWidth != p_width || m_walkHeight != p_height) {
		m_walkWidth = p_width;
		m_walkHeight = p_height;
		if (m_ground.m_ground != 0) {
			delete[] m_ground.m_ground;
		}
		m_ground.m_width = p_width;
		m_ground.m_height = p_height;
		m_ground.m_ground = new CGround[p_width * p_height];
		if (m_walkBits != 0) {
			delete[] m_walkBits;
		}
		m_walkBits = new unsigned char[m_walkWidth * m_walkHeight];
	}
}

// FUNCTION: LEMBALL 0x004304e0
unsigned short CMap::GetZ(int p_x, int p_y, CMover** p_mover)
{
	CMap* map = this;
	if (p_mover != 0) {
		int blockX = p_x >> 4;
		int blockY = p_y >> 4;
		if ((m_ground.m_ground[blockY * m_ground.m_width + blockX].m_collision & 0x10) != 0) {
			int height;
			CMover* mover = m_ai->FindMoverHeight(p_x, p_y, height);
			if (mover != 0) {
				*p_mover = mover;
				return (unsigned short) height;
			}
		}
	}
	int blockX = p_x >> 4;
	int blockY = p_y >> 4;
	if (p_x >= 0 && p_y >= 0 && blockX < m_ground.m_width && blockY < m_ground.m_height) {
		return m_ground.m_ground[blockY * m_ground.m_width + blockX].GetZ(p_x & 0xf, p_y & 0xf);
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
		y = 8;
		do {
			blockX = 0;
			if (m_walkWidth > 0) {
				x = 8;
				do {
					*walkBits = 0;

					if (8 < (int) y) {
						if (((((int) x < 8) || (adjacentBlock = blockY - 1, adjacentBlock < 0)) ||
							 m_ground.m_width <= blockX) ||
							(m_ground.m_height <= adjacentBlock)) {
							collision = 3;
						}
						else {
							ground = m_ground.GetGroundCell(blockX, adjacentBlock);
							collision = ground->m_collision;
						}
						if ((collision & 0x25) == 0) {
							collision = m_ground.GetZ(x, y - 8);
							firstHeight = collision;
							coordinate = y - 9;
							blockCoordinate = (int) x >> 4;
							adjacentBlock = (int) coordinate >> 4;
							if (((int) x < 0) || ((int) coordinate < 0) || m_ground.m_width <= blockCoordinate ||
								m_ground.m_height <= adjacentBlock) {
								z = 0;
							}
							else {
								lowCoordinate = x & 0xf;
								low = coordinate & 0xf;
								z = m_ground.GetGroundCell(blockCoordinate, adjacentBlock)->GetZ(lowCoordinate, low);
							}
							secondHeight = z;
							if (secondHeight <= firstHeight + 0xf) {
								*walkBits |= 0x10;
							}
							if (firstHeight <= secondHeight + 0xf) {
								*walkBits |= 1;
							}
						}
					}

					if (blockX < m_walkWidth - 1) {
						nextBlock = blockX + 1;
						if (((nextBlock < 0) || ((int) y < 8)) ||
							((m_ground.m_width <= nextBlock) || (m_ground.m_height <= blockY))) {
							collision = 3;
						}
						else {
							ground = m_ground.GetGroundCell(nextBlock, blockY);
							collision = ground->m_collision;
						}
						if ((collision & 0x25) == 0) {
							coordinate = x + 7;
							currentBlockY = (int) y >> 4;
							blockCoordinate = (int) coordinate >> 4;
							if (((((int) coordinate < 0) || ((int) y < 0)) || m_ground.m_width <= blockCoordinate) ||
								m_ground.m_height <= currentBlockY) {
								collision = 0;
							}
							else {
								low = coordinate & 0xf;
								lowCoordinate = y & 0xf;
								collision =
									m_ground.GetGroundCell(blockCoordinate, currentBlockY)->GetZ(low, lowCoordinate);
							}
							firstHeight = collision;
							nextBlock = (int) (x + 8) >> 4;
							if (((((int) (x + 8) < 0) || ((int) y < 0)) || m_ground.m_width <= nextBlock) ||
								(m_ground.m_height <= currentBlockY)) {
								z = 0;
							}
							else {
								lowCoordinate = y & 0xf;
								lowBlock = 0;
								z = m_ground.GetGroundCell(nextBlock, currentBlockY)->GetZ(lowBlock, lowCoordinate);
							}
							secondHeight = z;
							if (secondHeight <= firstHeight + 0xf) {
								*walkBits |= 0x40;
							}
							if (firstHeight <= secondHeight + 0xf) {
								*walkBits |= 4;
							}
						}
					}

					if (blockY < m_walkHeight - 1) {
						if (((((int) x < 8) || (nextBlock = blockY + 1, nextBlock < 0)) ||
							 m_ground.m_width <= blockX) ||
							m_ground.m_height <= nextBlock) {
							collision = 3;
						}
						else {
							ground = m_ground.GetGroundCell(blockX, nextBlock);
							collision = ground->m_collision;
						}
						if ((collision & 0x25) == 0) {
							blockCoordinate = (int) x >> 4;
							nextBlock = (int) (y + 7) >> 4;
							if (((((int) x < 0) || ((int) (y + 7) < 0)) || m_ground.m_width <= blockCoordinate) ||
								m_ground.m_height <= nextBlock) {
								collision = 0;
							}
							else {
								low = x & 0xf;
								lowCoordinate = (y + 7) & 0xf;
								collision =
									m_ground.GetGroundCell(blockCoordinate, nextBlock)->GetZ(low, lowCoordinate);
							}
							firstHeight = collision;
							nextBlock = (int) (y + 8) >> 4;
							if (((((int) x < 0) || ((int) (y + 8) < 0)) || m_ground.m_width <= blockCoordinate) ||
								m_ground.m_height <= nextBlock) {
								z = 0;
							}
							else {
								lowBlock = 0;
								low = x & 0xf;
								z = m_ground.GetGroundCell(blockCoordinate, nextBlock)->GetZ(low, lowBlock);
							}
							secondHeight = z;
							if (secondHeight <= firstHeight + 0xf) {
								*walkBits |= 0x20;
							}
							if (firstHeight <= secondHeight + 0xf) {
								*walkBits |= 2;
							}
						}
					}

					if (8 < (int) x) {
						if (((blockX - 1 < 0) || ((int) y < 8)) ||
							((blockCoordinate = m_ground.m_width, blockCoordinate <= blockX - 1) ||
							 m_ground.m_height <= blockY)) {
							collision = 3;
						}
						else {
							collision = m_ground.m_ground[blockCoordinate * blockY + blockX - 1].m_collision;
						}
						if ((collision & 0x25) == 0) {
							nextBlock = (int) (x - 8) >> 4;
							currentBlockY = (int) y >> 4;
							if (((((int) (x - 8) < 0) || ((int) y < 0)) || m_ground.m_width <= nextBlock) ||
								m_ground.m_height <= currentBlockY) {
								collision = 0;
							}
							else {
								lowCoordinate = y & 0xf;
								lowBlock = 0;
								collision =
									m_ground.GetGroundCell(nextBlock, currentBlockY)->GetZ(lowBlock, lowCoordinate);
							}
							westHeight = collision;
							nextBlock = (int) (x - 9) >> 4;
							if (((((int) (x - 9) < 0) || ((int) y < 0)) ||
								 ((m_ground.m_width <= nextBlock) || m_ground.m_height <= currentBlockY))) {
								z = 0;
							}
							else {
								z = (m_ground.m_ground + m_ground.m_width * currentBlockY + nextBlock)
										->GetZ((x - 9) & 0xf, y & 0xf);
							}
							secondHeight = z;
							if (secondHeight <= westHeight + 0xf) {
								*walkBits |= 0x80;
							}
							if (westHeight <= secondHeight + 0xf) {
								*walkBits |= 8;
							}
						}
					}

					x += 0x10;
					blockX++;
					walkBits++;
				} while (blockX < m_walkWidth);
			}
			y += 0x10;
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
void CMap::LoadLevelName(LoadGroundName* p_data, unsigned long p_dataSize)
{
	SetLevelName((char*) p_data);
}

// FUNCTION: LEMBALL 0x00430e90
void CMap::LoadDefaultBlox(class LoadDefaultBlox* p_data, unsigned long p_dataSize)
{
	unsigned short* data = (unsigned short*) p_data;
	unsigned int defaultBloxData = data[1];
	unsigned int defaultBlox = data[0];
	m_defaultBlox = (eObjectType) defaultBlox;
	m_defaultBloxData = defaultBloxData;
}

// FUNCTION: LEMBALL 0x00430eb0
bool ValidateDefaultBloxData(eObjectType p_type, unsigned short* p_data)
{
	unsigned short data = *p_data;
	switch (p_type) {
	case TERRAIN_TREE:
		if (*p_data >= g_wDefaultBloxLimit0202) {
			data = 0;
		}
		break;
	case TERRAIN_BLOX_1:
		if (g_wDefaultBloxLimit0206 <= *p_data) {
			data = 0;
		}
		break;
	case TERRAIN_BLOX_2:
		if (g_wDefaultBloxLimit0207 <= *p_data) {
			data = 0;
		}
		break;
	case TERRAIN_BLOX_3_SLOPE_SW_STEEP:
		if (*p_data >= g_wDefaultBloxLimit0208) {
			data = 0;
		}
		break;
	case TERRAIN_BLOX_4:
		if (*p_data >= g_wDefaultBloxLimit0209) {
			data = 0;
		}
		break;
	case TERRAIN_BLOX_5:
		if (*p_data >= g_wDefaultBloxLimit020A) {
			data = 0;
		}
		break;
	case TERRAIN_BLOX_6:
		if (*p_data >= g_wDefaultBloxLimit020B) {
			data = 0;
		}
		break;
	case TERRAIN_BLOX_7:
		if (g_wDefaultBloxLimit020C <= *p_data) {
			data = 0;
		}
		break;
	case TERRAIN_BLOX_8_SLOPE_SE_STEEP:
		if (g_wDefaultBloxLimit020D <= *p_data) {
			data = 0;
		}
		break;
	case TERRAIN_BLOX_14_SLOPE_SW_SHALLOW:
		if (g_wDefaultBloxLimit020E <= *p_data) {
			data = 0;
		}
		break;
	case TERRAIN_BLOX_15_SLOPE_SE_SHALLOW:
		if (g_wDefaultBloxLimit020F <= *p_data) {
			data = 0;
		}
		break;
	case TERRAIN_ANIM:
		if (*p_data >= g_wDefaultBloxLimit0210) {
			data = 0;
		}
		break;
	case TERRAIN_0x214:
		if (g_wDefaultBloxLimit0214 <= *p_data) {
			data = 0;
		}
		break;
	case TERRAIN_FLAME:
		return true;
	case TERRAIN_ELECTRIC:
	case TERRAIN_CONVEYOR_VARIANT_A:
	case TERRAIN_CONVEYOR_VARIANT_B:
		return true;
	case TERRAIN_EMBERS:
		if (g_wDefaultBloxLimit0217 <= *p_data) {
			data = 0;
		}
	}
	if (data != *p_data) {
		*p_data = data;
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
unsigned short g_wDefaultBloxLimit0206 = 0;

// GLOBAL: LEMBALL 0x0049e4ec
unsigned short g_wDefaultBloxLimit0207 = 0;

// GLOBAL: LEMBALL 0x0049e4f0
unsigned short g_wDefaultBloxLimit0208 = 0;

// GLOBAL: LEMBALL 0x0049e4f4
unsigned short g_wDefaultBloxLimit0209 = 0;

// GLOBAL: LEMBALL 0x0049e4f8
unsigned short g_wDefaultBloxLimit020A = 0;

// GLOBAL: LEMBALL 0x0049e4fc
unsigned short g_wDefaultBloxLimit020B = 0;

// GLOBAL: LEMBALL 0x0049e500
unsigned short g_wDefaultBloxLimit020C = 0;

// GLOBAL: LEMBALL 0x0049e504
unsigned short g_wDefaultBloxLimit020D = 0;

// GLOBAL: LEMBALL 0x0049e51c
unsigned short g_wDefaultBloxLimit020E = 0;

// GLOBAL: LEMBALL 0x0049e520
unsigned short g_wDefaultBloxLimit020F = 0;

// GLOBAL: LEMBALL 0x0049e528
unsigned short g_wDefaultBloxLimit0214 = 0;

// GLOBAL: LEMBALL 0x0049e534
unsigned short g_wDefaultBloxLimit0210 = 0;

// GLOBAL: LEMBALL 0x0049e538
unsigned short g_wDefaultBloxLimit0202 = 0;

// GLOBAL: LEMBALL 0x0049e544
unsigned short g_wDefaultBloxLimit0217 = 1;
