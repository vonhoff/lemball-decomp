#ifndef LEMBALL_AI_OBJECTS_COLLECTABLE_H
#define LEMBALL_AI_OBJECTS_COLLECTABLE_H

#include "../../Common.h"
#include "../Base/GlobalGameObject.h" // complete type

// SIZE 0x13c
// VTABLE: LEMBALL 0x004959c0
class Collectable : public GlobalGameObject {
public:
	Collectable(int p_arg0, int p_arg1, int p_arg2, eObjectType p_arg3);
	virtual bool Process();    // vtable+0x14
	virtual void DoActivate(); // vtable+0x10c
	virtual void Restart();    // vtable+0x104
	virtual void SetSfx();     // vtable+0x110
	virtual int Collected();   // vtable+0x114
	virtual ~Collectable();    // vtable+0x00
	Collectable();

private:
	unsigned int m_enabled; // 0x138
};

// SYNTHETIC: LEMBALL 0x00423060
// Collectable::`scalar deleting destructor'

#endif
