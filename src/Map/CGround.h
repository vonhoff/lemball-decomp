#ifndef LEMBALL_MAP_GROUND_CGROUND_H
#define LEMBALL_MAP_GROUND_CGROUND_H

#include "Gameplay/Objects/ObjectTypes.h"

#define GROUND_BLOCK_PIXEL_SIZE 16
#define GROUND_BLOCK_PIXEL_HALF_SIZE (GROUND_BLOCK_PIXEL_SIZE / 2)
#define GROUND_BLOCK_PIXEL_SHIFT 4
#define GROUND_BLOCK_PIXEL_MASK 0xf
#define GROUND_HIT_MASK_SIZE (GROUND_BLOCK_PIXEL_SIZE * 2)
#define GROUND_HIT_MASK_LAST_INDEX (GROUND_HIT_MASK_SIZE - 1)

#define GROUND_COLLISION_NONE 0
#define GROUND_COLLISION_BLOCKS_WALKING 0x01
#define GROUND_COLLISION_BLOCKS_BULLETS 0x02
#define GROUND_COLLISION_HAZARD 0x04
#define GROUND_COLLISION_MOVER_PRESENT 0x10
#define GROUND_COLLISION_SPECIAL_RENDER 0x20
#define GROUND_COLLISION_OBJECT_INTERACTION 0x8000
#define GROUND_COLLISION_OUT_OF_BOUNDS (GROUND_COLLISION_BLOCKS_WALKING | GROUND_COLLISION_BLOCKS_BULLETS)

// SIZE 0x0c
class CGround {
public:
	CGround();
	bool IsHit(int p_x, int p_y, unsigned int p_includeSpecial);
	unsigned short GetZ(int p_x, int p_y);
	void SetCollision();

	friend class CBullet;
	friend class CAI;
	friend class CPlayerLemming;
	friend class CGroundArray;
	friend class CGameObject;
	friend class CMap;
	friend class CDuplicator;
	friend class CFlag;
	friend class CDoor;
	friend class CIce;
	friend class CInvisibleSwitch;
	friend class CMine;
	friend class CMaze;
	friend class CRocket;
	friend class CLaser;
	friend class CHand;
	friend class CGroundAnim;
	friend class CTrampoline;
	friend class CMover;
	friend class CLift;
	friend class CPaintGun;
	friend class C2D;

public:
	eObjectType m_objectType;    // 0x00
	unsigned short m_objectData; // 0x04
	unsigned short m_collision;  // 0x06
	unsigned short m_height;     // 0x08
	unsigned short m_cliff;      // 0x0a
};

#endif
