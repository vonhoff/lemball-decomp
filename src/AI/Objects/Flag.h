#ifndef LEMBALL_AI_OBJECTS_FLAG_H
#define LEMBALL_AI_OBJECTS_FLAG_H

#include "../../Common.h"
#include "Collectable.h" // complete type

// SIZE 0x13c
// VTABLE: LEMBALL 0x00495720
class Flag : public Collectable {
public:
	virtual bool Process();  // vtable+0x14
	virtual int Collected(); // vtable+0x114
	virtual void SetSfx();   // vtable+0x110
	virtual ~Flag();         // vtable+0x00
};

// SYNTHETIC: LEMBALL 0x00422fb0
// Flag::`scalar deleting destructor'

#endif
