#ifndef LEMBALL_MAP_GROUND_CGROUND_H
#define LEMBALL_MAP_GROUND_CGROUND_H

#include "../../AI/Base/ObjectTypes.h"

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
