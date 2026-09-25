#ifndef LEMBALL_AI_OBJECTS_CBONUS_H
#define LEMBALL_AI_OBJECTS_CBONUS_H

#include "AI/Base/ObjectTypes.h"
#include "CCollectable.h"

// SIZE 0x13c
// VTABLE: LEMBALL 0x00495870
class CBonus : public CCollectable {
public:
	CBonus(int p_x, int p_y, int p_z, eObjectType p_objectType) : CCollectable(p_x, p_y, p_z, p_objectType) {}
	virtual int Collected(); // vtable+0x114
	virtual void SetSFX();   // vtable+0x110
};

// SYNTHETIC: LEMBALL 0x00422fe0
// CBonus::`scalar deleting destructor'

#endif
