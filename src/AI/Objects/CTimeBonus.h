#ifndef LEMBALL_AI_OBJECTS_CTIMEBONUS_H
#define LEMBALL_AI_OBJECTS_CTIMEBONUS_H

#include "AI/Base/ObjectTypes.h"
#include "CCollectable.h" // complete type

// SIZE 0x13c
// VTABLE: LEMBALL 0x004955d0
class CTimeBonus : public CCollectable {
public:
	CTimeBonus(int p_x, int p_y, int p_z, eObjectType p_objectType) : CCollectable(p_x, p_y, p_z, p_objectType) {}
	virtual int Collected(); // vtable+0x114
	virtual void SetSfx();   // vtable+0x110
};

// SYNTHETIC: LEMBALL 0x00423010
// CTimeBonus::`scalar deleting destructor'

#endif
