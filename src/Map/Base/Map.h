#ifndef LEMBALL_MAP_BASE_MAP_H
#define LEMBALL_MAP_BASE_MAP_H

#include "../../Common.h"
#include "../Ground/GroundArray.h" // complete type

// SIZE 0x58
class Map {
public:
	Map();
	unsigned char GetWalk(int p_x, int p_y);
	unsigned short GetZ(int p_x, int p_y, Mover** p_mover);
	void CalculateCliff();
	void CreateWalkBits();
	void GameToScreen(int p_gameX, int p_gameY, int& p_screenX, int& p_screenY);
	void GameToScreen(int& p_x, int& p_y);
	void LoadDefaultBlox(class LoadDefaultBlox* p_data, unsigned long p_dataSize);
	void LoadLevel(LoadGroundSurfaceData* p_data, unsigned long p_dataSize, unsigned char p_skip);
	void LoadLevelName(LoadGroundName* p_data, unsigned long p_dataSize);
	void ReSize(int p_width, int p_height);
	void Restart();
	void ScreenToGame(int p_screenX, int p_screenY, int& p_gameX, int& p_gameY);
	void SetLevelName(char* p_name);
	void SetTerrain(int p_x, int p_y, eObjectType p_objectType, int p_data);
	~Map();

	friend class Ai;
	friend class Bullet;
	friend class PlayerLemming;
	friend class Tower;
	friend class Enemy;
	friend class Ammo;
	friend class Key;
	friend class Balloon;
	friend class Catapult;
	friend class Collectable;
	friend class Crate;
	friend class Duplicator;
	friend class Ball;

private:
	unsigned int m_reserved;   // 0x00
	int m_orientation;         // 0x04
	GroundArray m_ground;      // 0x08
	undefined4 m_unk0x18;      // 0x18
	int m_walkWidth;           // 0x1c
	int m_walkHeight;          // 0x20
	char m_levelName[33];      // 0x24
	unsigned char* m_walkBits; // 0x48
	eObjectType m_defaultBlox; // 0x4c
	int m_defaultBloxData;     // 0x50
	Ai* m_ai;                  // 0x54
};

extern Map* g_pMap;
extern Map* g_pActiveMap;
extern Map* g_pCurrentMap;
extern void* g_apValidateDefaultBloxDataCases[25];
extern unsigned short g_wDefaultBloxLimit0206;
extern unsigned short g_wDefaultBloxLimit0207;
extern unsigned short g_wDefaultBloxLimit0208;
extern unsigned short g_wDefaultBloxLimit0209;
extern unsigned short g_wDefaultBloxLimit020A;
extern unsigned short g_wDefaultBloxLimit020B;
extern unsigned short g_wDefaultBloxLimit020C;
extern unsigned short g_wDefaultBloxLimit020D;
extern unsigned short g_wDefaultBloxLimit020E;
extern unsigned short g_wDefaultBloxLimit020F;
extern unsigned short g_wDefaultBloxLimit0214;
extern unsigned short g_wDefaultBloxLimit0210;
extern unsigned short g_wDefaultBloxLimit0202;
extern unsigned short g_wDefaultBloxLimit0217;
#endif
