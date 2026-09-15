#ifndef LEMBALL_AI_OBJECTS_FLAG_H
#define LEMBALL_AI_OBJECTS_FLAG_H

#include "../../Common.h"
#include "Collectable.h" // complete type

// SIZE 0x13c
// VTABLE: LEMBALL 0x00495720
class Flag : public Collectable {
public:
	Flag(int p_x, int p_y, int p_z, eObjectType p_objectType) : Collectable(p_x, p_y, p_z, p_objectType)
	{
		m_objectType = p_objectType;
	}
	virtual bool Process();  // vtable+0x14
	virtual int Collected(); // vtable+0x114
	virtual void SetSfx();   // vtable+0x110
	virtual ~Flag();         // vtable+0x00
};

// SYNTHETIC: LEMBALL 0x00422fb0
// Flag::`scalar deleting destructor'

#endif
