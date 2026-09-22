#ifndef LEMBALL_MAP_GROUND_CGROUNDARRAY_H
#define LEMBALL_MAP_GROUND_CGROUNDARRAY_H

#include "CGround.h"

// SIZE 0x10
class CGroundArray {
public:
	CGround* GetGroundCell(int p_x, int p_y);
	unsigned short GetZ(int p_x, int p_y);
	void Clear();
	void SetCollision(int p_x, int p_y, int p_collision);

	friend class CBullet;
	friend class CAI;
	friend class CTrapDoor;
	friend class CTrapDoorManager;
	friend class CPlayerLemmingGroupManager;
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
	friend class CMap;
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
	unsigned int m_reserved; // 0x00
	CGround* m_ground;       // 0x04
	int m_width;             // 0x08
	int m_height;            // 0x0c
};

#endif
