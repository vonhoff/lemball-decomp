#include "Map.h"

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
// STUB: LEMBALL 0x00430440
void Map::ReSize(int p_width, int p_height)
{
}

// 68K 0x10900630 GetZ__4CMapFiiPP6CMover
// STUB: LEMBALL 0x004304e0
unsigned short Map::GetZ(int p_x, int p_y, Mover** p_mover)
{
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
	if (m_walkHeight > 0) {
		unsigned char* walkBits = m_walkBits;
		int y = 8;
		int blockY = 0;
		while (blockY < m_walkHeight) {
			if (m_walkWidth > 0) {
				int x = 8;
				int blockX = 0;
				while (blockX < m_walkWidth) {
					*walkBits = 0;

					if (y > 8) {
						unsigned short collision = 3;
						int previousBlockY = blockY - 1;
						if (x >= 8 && previousBlockY >= 0 && blockX < m_ground.m_width &&
							previousBlockY < m_ground.m_height) {
							collision = m_ground.GetGroundCell(blockX, previousBlockY)->m_collision;
						}
						if ((collision & 0x25) == 0) {
							unsigned short currentZ = m_ground.GetZ(x, y - 8);
							unsigned short previousZ = m_ground.GetZ(x, y - 9);
							if (currentZ + 0xf >= previousZ) {
								*walkBits |= 0x10;
							}
							if (previousZ + 0xf >= currentZ) {
								*walkBits |= 1;
							}
						}
					}

					if (blockX < m_walkWidth - 1) {
						unsigned short collision = 3;
						int nextBlockX = blockX + 1;
						if (y >= 8 && nextBlockX >= 0 && nextBlockX < m_ground.m_width && blockY < m_ground.m_height) {
							collision = m_ground.GetGroundCell(nextBlockX, blockY)->m_collision;
						}
						if ((collision & 0x25) == 0) {
							unsigned short currentZ = 0;
							int currentX = x + 7;
							int currentBlockX = currentX >> 4;
							int currentBlockY = y >> 4;
							if (currentX >= 0 && y >= 0 && currentBlockX < m_ground.m_width &&
								currentBlockY < m_ground.m_height) {
								currentZ =
									m_ground.GetGroundCell(currentBlockX, currentBlockY)->GetZ(currentX & 0xf, y & 0xf);
							}
							unsigned short nextZ = 0;
							int nextX = x + 8;
							nextBlockX = nextX >> 4;
							if (nextX >= 0 && y >= 0 && nextBlockX < m_ground.m_width &&
								currentBlockY < m_ground.m_height) {
								nextZ = m_ground.GetGroundCell(nextBlockX, currentBlockY)->GetZ(nextX & 0xf, y & 0xf);
							}
							if (currentZ + 0xf >= nextZ) {
								*walkBits |= 0x40;
							}
							if (nextZ + 0xf >= currentZ) {
								*walkBits |= 4;
							}
						}
					}

					if (blockY < m_walkHeight - 1) {
						unsigned short collision = 3;
						int nextBlockY = blockY + 1;
						if (x >= 8 && nextBlockY >= 0 && blockX < m_ground.m_width && nextBlockY < m_ground.m_height) {
							collision = m_ground.GetGroundCell(blockX, nextBlockY)->m_collision;
						}
						if ((collision & 0x25) == 0) {
							unsigned short currentZ = 0;
							int currentY = y + 7;
							int currentBlockX = x >> 4;
							int currentBlockY = currentY >> 4;
							if (x >= 0 && currentY >= 0 && currentBlockX < m_ground.m_width &&
								currentBlockY < m_ground.m_height) {
								currentZ =
									m_ground.GetGroundCell(currentBlockX, currentBlockY)->GetZ(x & 0xf, currentY & 0xf);
							}
							unsigned short nextZ = 0;
							int nextY = y + 8;
							nextBlockY = nextY >> 4;
							if (x >= 0 && nextY >= 0 && currentBlockX < m_ground.m_width &&
								nextBlockY < m_ground.m_height) {
								nextZ = m_ground.GetGroundCell(currentBlockX, nextBlockY)->GetZ(x & 0xf, nextY & 0xf);
							}
							if (currentZ + 0xf >= nextZ) {
								*walkBits |= 0x20;
							}
							if (nextZ + 0xf >= currentZ) {
								*walkBits |= 2;
							}
						}
					}

					if (x > 8) {
						unsigned short collision = 3;
						int previousBlockX = blockX - 1;
						if (previousBlockX >= 0 && y >= 8 && previousBlockX < m_ground.m_width &&
							blockY < m_ground.m_height) {
							collision = m_ground.GetGroundCell(previousBlockX, blockY)->m_collision;
						}
						if ((collision & 0x25) == 0) {
							unsigned short currentZ = 0;
							int currentX = x - 8;
							int currentBlockX = currentX >> 4;
							int currentBlockY = y >> 4;
							if (currentX >= 0 && y >= 0 && currentBlockX < m_ground.m_width &&
								currentBlockY < m_ground.m_height) {
								currentZ =
									m_ground.GetGroundCell(currentBlockX, currentBlockY)->GetZ(currentX & 0xf, y & 0xf);
							}
							unsigned short previousZ = 0;
							int previousX = x - 9;
							previousBlockX = previousX >> 4;
							if (previousX >= 0 && y >= 0 && previousBlockX < m_ground.m_width &&
								currentBlockY < m_ground.m_height) {
								previousZ = m_ground.GetGroundCell(previousBlockX, currentBlockY)
												->GetZ(previousX & 0xf, y & 0xf);
							}
							if (currentZ + 0xf >= previousZ) {
								*walkBits |= 0x80;
							}
							if (previousZ + 0xf >= currentZ) {
								*walkBits |= 8;
							}
						}
					}

					walkBits++;
					x += 0x10;
					blockX++;
				}
			}
			y += 0x10;
			blockY++;
		}
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
// STUB: LEMBALL 0x00430a50
void Map::ScreenToGame(int p_screenX, int p_screenY, int& p_gameX, int& p_gameY)
{
}

// 68K 0x10900b1a GameToScreen__4CMapFiiRiRi
// STUB: LEMBALL 0x00430be0
void Map::GameToScreen(int p_gameX, int p_gameY, int& p_screenX, int& p_screenY)
{
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
