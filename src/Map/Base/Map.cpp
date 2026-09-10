#include "Map.h"

#include "../../AI/Navigation/Ai.h"

// GLOBAL: LEMBALL 0x0049e4e0
Map* g_pActiveMap = 0;

// GLOBAL: LEMBALL 0x0049e4e4
Map* g_pCurrentMap = 0;

// 68K 0x10900470 __ct__4CMapFv
// FUNCTION: LEMBALL 0x004303c0
Map::Map()
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

// 68K 0x109004b2 Restart__4CMapFv
// FUNCTION: LEMBALL 0x004303f0
void Map::Restart()
{
	m_orientation = 0;
	m_levelName[0] = '\0';
	m_defaultBlox = (eObjectType) 0x209;
	m_defaultBloxData = 0;
	m_ground.Clear();
}

// 68K 0x109004f2 __dt__4CMapFv
// FUNCTION: LEMBALL 0x00430410
Map::~Map()
{
	if (m_walkBits != 0) {
		delete[] m_walkBits;
	}
	if (m_ground.m_ground != 0) {
		delete[] m_ground.m_ground;
	}
}

// 68K 0x1090055c ReSize__4CMapFii
// FUNCTION: LEMBALL 0x00430440
void Map::ReSize(int p_width, int p_height)
{
	if (m_walkWidth != p_width || m_walkHeight != p_height) {
		m_walkWidth = p_width;
		m_walkHeight = p_height;
		if (m_ground.m_ground != 0) {
			delete[] m_ground.m_ground;
		}
		m_ground.m_width = p_width;
		m_ground.m_height = p_height;
		m_ground.m_ground = new Ground[p_width * p_height];
		if (m_walkBits != 0) {
			delete[] m_walkBits;
		}
		m_walkBits = new unsigned char[m_walkWidth * m_walkHeight];
	}
}

// 68K 0x10900630 GetZ__4CMapFiiPP6CMover
// FUNCTION: LEMBALL 0x004304e0
unsigned short Map::GetZ(int p_x, int p_y, Mover** p_mover)
{
	if (p_mover != 0) {
		int blockX = p_x >> 4;
		int blockY = p_y >> 4;
		if ((m_ground.m_ground[blockY * m_ground.m_width + blockX].m_collision & 0x10) != 0) {
			int height;
			Mover* mover = m_ai->FindMoverHeight(p_x, p_y, height);
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

// 68K 0x109006da GetWalk__4CMapFii
// FUNCTION: LEMBALL 0x004305f0
unsigned char Map::GetWalk(int p_x, int p_y)
{
	if (p_x >= 0 && p_y >= 0 && p_x < m_walkWidth && m_walkHeight > p_y) {
		return m_walkBits[p_y * m_walkWidth + p_x];
	}
	return '\0';
}

// 68K 0x1090072c CreateWalkBits__4CMapFv
// FUNCTION: LEMBALL 0x00430620
void Map::CreateWalkBits()
{
	unsigned short collision;
	unsigned short z;
	int adjacentBlock;
	Ground* ground;
	unsigned int low;
	unsigned int coordinate;
	int nextBlock;
	int blockCoordinate;
	int currentBlockY;
	unsigned int y;
	unsigned int x;
	unsigned int lowCoordinate;
	int lowBlock;
	unsigned char* walkBits;
	int blockX;
	int blockY;

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
					blockCoordinate = (int) x >> 4;

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
							coordinate = y - 9;
							if (((int) x < 0) || ((int) coordinate < 0) || m_ground.m_width <= blockCoordinate ||
								m_ground.m_height <= (int) coordinate >> 4) {
								z = 0;
							}
							else {
								low = coordinate & 0xf;
								lowCoordinate = x & 0xf;
								ground = m_ground.GetGroundCell(blockCoordinate, (int) coordinate >> 4);
								z = ground->GetZ(lowCoordinate, low);
							}
							if (z <= collision + 0xf) {
								*walkBits |= 0x10;
							}
							if (collision <= z + 0xf) {
								*walkBits |= 1;
							}
						}
					}

					blockCoordinate = (int) x >> 4;
					currentBlockY = (int) y >> 4;
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
							if (((((int) coordinate < 0) || ((int) y < 0)) ||
								 m_ground.m_width <= (int) coordinate >> 4) ||
								m_ground.m_height <= currentBlockY) {
								collision = 0;
							}
							else {
								low = coordinate & 0xf;
								lowCoordinate = y & 0xf;
								ground = m_ground.GetGroundCell((int) coordinate >> 4, currentBlockY);
								collision = ground->GetZ(low, lowCoordinate);
							}
							nextBlock = (int) (x + 8) >> 4;
							if (((((int) (x + 8) < 0) || ((int) y < 0)) || m_ground.m_width <= nextBlock) ||
								(m_ground.m_height <= currentBlockY)) {
								z = 0;
							}
							else {
								lowCoordinate = y & 0xf;
								lowBlock = 0;
								ground = m_ground.GetGroundCell(nextBlock, currentBlockY);
								z = ground->GetZ(lowBlock, lowCoordinate);
							}
							if (z <= collision + 0xf) {
								*walkBits |= 0x40;
							}
							if (collision <= z + 0xf) {
								*walkBits |= 4;
							}
						}
					}

					if (blockY < m_walkHeight - 1) {
						nextBlock = blockY + 1;
						if (((((int) x < 8) || (nextBlock < 0)) || m_ground.m_width <= blockX) ||
							m_ground.m_height <= nextBlock) {
							collision = 3;
						}
						else {
							ground = m_ground.GetGroundCell(blockX, nextBlock);
							collision = ground->m_collision;
						}
						if ((collision & 0x25) == 0) {
							nextBlock = (int) (y + 7) >> 4;
							if (((((int) x < 0) || ((int) (y + 7) < 0)) || m_ground.m_width <= blockCoordinate) ||
								m_ground.m_height <= nextBlock) {
								collision = 0;
							}
							else {
								low = x & 0xf;
								lowCoordinate = (y + 7) & 0xf;
								ground = m_ground.GetGroundCell(blockCoordinate, nextBlock);
								collision = ground->GetZ(low, lowCoordinate);
							}
							nextBlock = (int) (y + 8) >> 4;
							if (((((int) x < 0) || ((int) (y + 8) < 0)) || m_ground.m_width <= blockCoordinate) ||
								m_ground.m_height <= nextBlock) {
								z = 0;
							}
							else {
								lowBlock = 0;
								low = x & 0xf;
								ground = m_ground.GetGroundCell(blockCoordinate, nextBlock);
								z = ground->GetZ(low, lowBlock);
							}
							if (z <= collision + 0xf) {
								*walkBits |= 0x20;
							}
							if (collision <= z + 0xf) {
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
							if (((((int) (x - 8) < 0) || ((int) y < 0)) || m_ground.m_width <= nextBlock) ||
								m_ground.m_height <= currentBlockY) {
								collision = 0;
							}
							else {
								lowCoordinate = y & 0xf;
								lowBlock = 0;
								ground = m_ground.GetGroundCell(nextBlock, currentBlockY);
								collision = ground->GetZ(lowBlock, lowCoordinate);
							}
							nextBlock = (int) (x - 9) >> 4;
							if (((((int) (x - 9) < 0) || ((int) y < 0)) ||
								 ((m_ground.m_width <= nextBlock) || m_ground.m_height <= currentBlockY))) {
								z = 0;
							}
							else {
								ground = m_ground.m_ground + m_ground.m_width * currentBlockY + nextBlock;
								z = ground->GetZ((x - 9) & 0xf, y & 0xf);
							}
							if (z <= collision + 0xf) {
								*walkBits |= 0x80;
							}
							if (collision <= z + 0xf) {
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

// 68K 0x10900994 SetTerrain__4CMapFii11eObjectTypei
// FUNCTION: LEMBALL 0x00430a20
void Map::SetTerrain(int p_x, int p_y, eObjectType p_objectType, int p_data)
{
	Ground* ground = m_ground.m_ground + m_ground.m_width * p_y + p_x;
	ground->m_objectType = p_objectType;
	ground->m_objectData = p_data;
	ground->SetCollision();
}

// 68K 0x10900a04 ScreenToGame__4CMapFiiRiRi
// FUNCTION: LEMBALL 0x00430a50
void Map::ScreenToGame(int p_screenX, int p_screenY, int& p_gameX, int& p_gameY)
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

// 68K 0x10900b1a GameToScreen__4CMapFiiRiRi
// FUNCTION: LEMBALL 0x00430be0
void Map::GameToScreen(int p_gameX, int p_gameY, int& p_screenX, int& p_screenY)
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

// 68K 0x10900c34 GameToScreen__4CMapFRiRi
// STUB: LEMBALL 0x00430ce0
void Map::GameToScreen(int& p_x, int& p_y)
{
}

// 68K 0x10900d48 LoadLevel__4CMapFP24tagLoadGroundSurfaceDataUlUc
// FUNCTION: LEMBALL 0x00430db0
void Map::LoadLevel(LoadGroundSurfaceData* p_data, unsigned long p_dataSize, unsigned char p_skip)
{
	int width = ((unsigned short*) p_data)[0];
	int height = ((unsigned short*) p_data)[1];
	p_data = (LoadGroundSurfaceData*) &((unsigned short*) p_data)[2];

	m_ground.Clear();
	ReSize(width, height);

	for (int y = 0; height > y; y++) {
		for (int x = 0; width > x; x++) {
			eObjectType objectType = (eObjectType) * (unsigned short*) p_data;
			p_data = (LoadGroundSurfaceData*) &((unsigned short*) p_data)[1];
			unsigned short objectData = *(unsigned short*) p_data;
			p_data = (LoadGroundSurfaceData*) &((unsigned short*) p_data)[1];
			unsigned short groundHeight = *(unsigned short*) p_data;
			p_data = (LoadGroundSurfaceData*) &((unsigned short*) p_data)[1];

			Ground* ground = m_ground.m_ground + m_ground.m_width * y + x;
			ground->m_objectType = objectType;
			ground->m_objectData = objectData;
			ground->SetCollision();

			m_ground.m_ground[m_ground.m_width * y + x].m_height = groundHeight;
		}
	}

	CreateWalkBits();
	CalculateCliff();
}

// 68K 0x10900e76 LoadLevelName__4CMapFP17tagLoadGroundNameUl
// FUNCTION: LEMBALL 0x00430e80
void Map::LoadLevelName(LoadGroundName* p_data, unsigned long p_dataSize)
{
	SetLevelName((char*) p_data);
}

// 68K 0x10900ec2 LoadDefaultBlox__4CMapFP18tagLoadDefaultBloxUl
// FUNCTION: LEMBALL 0x00430e90
void Map::LoadDefaultBlox(class LoadDefaultBlox* p_data, unsigned long p_dataSize)
{
	unsigned short* data = (unsigned short*) p_data;
	unsigned int defaultBloxData = data[1];
	unsigned int defaultBlox = data[0];
	m_defaultBlox = (eObjectType) defaultBlox;
	m_defaultBloxData = defaultBloxData;
}

// 68K 0x10900f30 SetLevelName__4CMapFPc
// FUNCTION: LEMBALL 0x00431010
void Map::SetLevelName(char* p_name)
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

// 68K 0x10900f74 CalculateCliff__4CMapFv
// STUB: LEMBALL 0x00431030
void Map::CalculateCliff()
{
}

// GLOBAL: LEMBALL 0x004a74b4
Map* g_pMap;

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
