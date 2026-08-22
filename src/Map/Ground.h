#ifndef LEMBALL_SCAFFOLD_MAP_GROUND_H
#define LEMBALL_SCAFFOLD_MAP_GROUND_H

#include "../Common.h"

// SIZE 0x0c
class Ground {
public:
	Ground();
	bool IsHit(int p_x, int p_y, unsigned char p_includeSpecial);
	unsigned short GetZ(int p_x, int p_y);
	void SetCollision();

private:
	eObjectType m_objectType;    // 0x00
	unsigned short m_objectData; // 0x04
	unsigned short m_collision;  // 0x06
	unsigned short m_height;     // 0x08
	unsigned short m_cliff;      // 0x0a
};

#endif
