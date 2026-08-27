#ifndef LEMBALL_AI_OBJECTS_SLINKY_H
#define LEMBALL_AI_OBJECTS_SLINKY_H

#include "../../Common.h"
#include "../Base/GameObject.h" // complete type

// SIZE 0x150
// VTABLE: LEMBALL 0x004932f8
class Slinky : public GameObject {
public:
	Slinky();
	bool GoodEndPt(const AiCoord& p_coordinate);
	virtual bool Move();    // vtable+0x44
	virtual bool Process(); // vtable+0x14
	virtual ~Slinky();      // vtable+0x00
	void Set(int p_minX, int p_maxX, int p_minY, int p_maxY);

private:
	int m_minX; // 0x124
	int m_minY; // 0x128
	int m_maxX; // 0x12c
	int m_maxY; // 0x130
};

// SYNTHETIC: LEMBALL 0x0040c1d0
// Slinky::`scalar deleting destructor'


#endif

