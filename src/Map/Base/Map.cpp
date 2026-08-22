#include "Map.h"

// 68K 0x10900470 __ct__4CMapFv
// STUB: LEMBALL 0x004303c0
Map::Map()
{
}

// 68K 0x109004b2 Restart__4CMapFv
// STUB: LEMBALL 0x004303f0
void Map::Restart()
{
}

// 68K 0x109004f2 __dt__4CMapFv
// STUB: LEMBALL 0x00430410
Map::~Map()
{
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
// STUB: LEMBALL 0x004305f0
unsigned char Map::GetWalk(int p_x, int p_y)
{
	return 0;
}

// 68K 0x1090072c CreateWalkBits__4CMapFv
// STUB: LEMBALL 0x00430620
void Map::CreateWalkBits()
{
}

// 68K 0x10900994 SetTerrain__4CMapFii11eObjectTypei
// STUB: LEMBALL 0x00430a20
void Map::SetTerrain(int p_x, int p_y, eObjectType p_objectType, int p_data)
{
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
// STUB: LEMBALL 0x00430db0
void Map::LoadLevel(LoadGroundSurfaceData* p_data, unsigned long p_dataSize, unsigned char p_skip)
{
}

// 68K 0x10900e76 LoadLevelName__4CMapFP17tagLoadGroundNameUl
// STUB: LEMBALL 0x00430e80
void Map::LoadLevelName(LoadGroundName* p_data, unsigned long p_dataSize)
{
}

// 68K 0x10900ec2 LoadDefaultBlox__4CMapFP18tagLoadDefaultBloxUl
// STUB: LEMBALL 0x00430e90
void Map::LoadDefaultBlox(class LoadDefaultBlox* p_data, unsigned long p_dataSize)
{
}

// 68K 0x10900f30 SetLevelName__4CMapFPc
// STUB: LEMBALL 0x00431010
void Map::SetLevelName(char* p_name)
{
}

// 68K 0x10900f74 CalculateCliff__4CMapFv
// STUB: LEMBALL 0x00431030
void Map::CalculateCliff()
{
}

// Confirmed class-scoped globals.
// GLOBAL: LEMBALL 0x004a74b4
Map* g_pMap;

// GLOBAL: LEMBALL 0x0049e4e8
unsigned short g_wDefaultBloxLimit0206;

// GLOBAL: LEMBALL 0x0049e4ec
unsigned short g_wDefaultBloxLimit0207;

// GLOBAL: LEMBALL 0x0049e4f0
unsigned short g_wDefaultBloxLimit0208;

// GLOBAL: LEMBALL 0x0049e4f4
unsigned short g_wDefaultBloxLimit0209;

// GLOBAL: LEMBALL 0x0049e4f8
unsigned short g_wDefaultBloxLimit020A;

// GLOBAL: LEMBALL 0x0049e4fc
unsigned short g_wDefaultBloxLimit020B;

// GLOBAL: LEMBALL 0x0049e500
unsigned short g_wDefaultBloxLimit020C;

// GLOBAL: LEMBALL 0x0049e504
unsigned short g_wDefaultBloxLimit020D;

// GLOBAL: LEMBALL 0x0049e51c
unsigned short g_wDefaultBloxLimit020E;

// GLOBAL: LEMBALL 0x0049e520
unsigned short g_wDefaultBloxLimit020F;

// GLOBAL: LEMBALL 0x0049e528
unsigned short g_wDefaultBloxLimit0214;

// GLOBAL: LEMBALL 0x0049e534
unsigned short g_wDefaultBloxLimit0210;

// GLOBAL: LEMBALL 0x0049e538
unsigned short g_wDefaultBloxLimit0202;

// GLOBAL: LEMBALL 0x0049e544
unsigned short g_wDefaultBloxLimit0217;
