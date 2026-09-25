#ifndef LEMBALL_AI_OBJECTS_CSLINKY_H
#define LEMBALL_AI_OBJECTS_CSLINKY_H

#include "../Base/AiCoord.h"
#include "../Base/CGameObject.h"
// SIZE 0x150
// VTABLE: LEMBALL 0x004932f8
class CSlinky : public CGameObject {
public:
	CSlinky();
	bool GoodEndPt(const AiCoord& p_coordinate);
	bool ContainsIntegerPoint(const int* p_xy);
	virtual bool Move();    // vtable+0x44
	virtual bool Process(); // vtable+0x14
	void Set(int p_minX, int p_maxX, int p_minY, int p_maxY);
	void GetBounds(int* p_minX, int* p_maxX, int* p_minY, int* p_maxY);

private:
	int m_minX;              // 0x124
	int m_minY;              // 0x128
	int m_maxX;              // 0x12c
	int m_maxY;              // 0x130
	unsigned int m_unk0x134; // 0x134
	AiCoord m_unk0x138;      // 0x138
	AiCoord m_unk0x144;      // 0x144
};

// SYNTHETIC: LEMBALL 0x0040c1d0
// CSlinky::`vector deleting destructor'

// SYNTHETIC: LEMBALL 0x0040c260
// CSlinky::~CSlinky

#endif
