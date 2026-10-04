#ifndef LEMBALL_MAP_BASE_CMAP_H
#define LEMBALL_MAP_BASE_CMAP_H

#include "Gameplay/Objects/ObjectTypes.h"
#include "CGroundArray.h"

class CAI;
class CMover;
enum {
	MAP_COORDINATE_MAX = 1024
};
enum {
	MAP_LEVEL_NAME_MAX_CHARACTERS = 32,
	MAP_LEVEL_NAME_BUFFER_SIZE = MAP_LEVEL_NAME_MAX_CHARACTERS + 1
};
enum eMapOrientation {
	MAP_ORIENTATION_ROTATION_0_DEGREES = 0,
	MAP_ORIENTATION_ROTATION_90_DEGREES = 1,
	MAP_ORIENTATION_ROTATION_180_DEGREES = 2,
	MAP_ORIENTATION_ROTATION_270_DEGREES = 3
};
enum {
	MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE = GROUND_BLOCK_PIXEL_SIZE / 2,
	MAP_PROJECTION_BLOCK_PIXEL_SIZE = GROUND_BLOCK_PIXEL_SIZE,
	MAP_PROJECTION_DOUBLE_BLOCK_PIXEL_SIZE = GROUND_BLOCK_PIXEL_SIZE * 2,
	MAP_PROJECTION_BLOCK_PLUS_HALF_PIXEL_SIZE = MAP_PROJECTION_BLOCK_PIXEL_SIZE + MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE
};
struct tagLoadGroundName;
struct tagLoadGroundSurfaceData;
struct tagLoadDefaultBlox;
// SIZE 0x58
class CMap {
public:
	CMap();
	unsigned char GetWalk(int p_x, int p_y);
	int TestWalkBit(int p_x, int p_y, unsigned char p_mask);
	unsigned short GetZ(int p_x, int p_y, CMover** p_mover);
	void CalculateCliff();
	void CreateWalkBits();
	void GameToScreen(int p_gameX, int p_gameY, int& p_screenX, int& p_screenY);
	void GameToScreen(int& p_x, int& p_y);
	void LoadDefaultBlox(tagLoadDefaultBlox* p_data, unsigned long p_dataSize);
	void LoadLevel(tagLoadGroundSurfaceData* p_data, unsigned long p_dataSize, unsigned char p_skip);
	void LoadLevelName(tagLoadGroundName* p_data, unsigned long p_dataSize);
	void ReSize(int p_width, int p_height);
	void Restart();
	void ScreenToGame(int p_screenX, int p_screenY, int& p_gameX, int& p_gameY);
	void ScreenToGame(int& p_x, int& p_y);
	void SetLevelName(char* p_name);
	void SetTerrain(int p_x, int p_y, eObjectType p_objectType, int p_data);
	~CMap();

	friend class CAI;
	friend class CTrapDoor;
	friend class CTrapDoorManager;
	friend class CPlayerLemmingGroupManager;
	friend class CBullet;
	friend class CPlayerLemming;
	friend class CTower;
	friend class CEnemy;
	friend class CAmmo;
	friend class CKey;
	friend class CBalloon;
	friend class CCatapult;
	friend class CCollectable;
	friend class CCrate;
	friend class CDuplicator;
	friend class CBall;
	friend class CFlag;
	friend class CDoor;
	friend class CIce;
	friend class CInvisibleSwitch;
	friend class CSwitch;
	friend class CHand;
	friend class CLift;
	friend class CSlinky;
	friend class CBalloonPost;
	friend class CMover;
	friend class CPaintGun;
	friend class CMaze;
	friend class CMine;
	friend class CRocket;
	friend class CLaser;
	friend class C2D;
	friend class CTrampoline;
	friend class CGroundAnim;
	friend class CGameObject;

private:
	unsigned int m_mapType; // 0x00
	int m_orientation;      // 0x04

public:
	CGroundArray m_ground; // 0x08

private:
	CAI* m_ownerAI;                               // 0x18
	int m_walkWidth;                              // 0x1c
	int m_walkHeight;                             // 0x20
	char m_levelName[MAP_LEVEL_NAME_BUFFER_SIZE]; // 0x24
	unsigned char* m_walkBits;                    // 0x48
	eObjectType m_defaultBlox;                    // 0x4c
	int m_defaultBloxData;                        // 0x50
	CAI* m_ai;                                    // 0x54
};

extern CMap* g_pMap;
extern CMap* g_pActiveMap;
extern CMap* g_pCurrentMap;
extern void* g_apValidateDefaultBloxDataCases[25];
extern unsigned short g_blox1FrameLimit;
extern unsigned short g_blox2FrameLimit;
extern unsigned short g_steepSwSlopeFrameLimit;
extern unsigned short g_blox4FrameLimit;
extern unsigned short g_blox5FrameLimit;
extern unsigned short g_blox6FrameLimit;
extern unsigned short g_blox7FrameLimit;
extern unsigned short g_steepSeSlopeFrameLimit;
extern unsigned short g_shallowSwSlopeFrameLimit;
extern unsigned short g_shallowSeSlopeFrameLimit;
extern unsigned short g_wCliffOnlyGroundDataLimit;
extern unsigned short g_groundAnimFrameLimit;
extern unsigned short g_treeFrameLimit;
extern unsigned short g_embersFrameLimit;
#endif
