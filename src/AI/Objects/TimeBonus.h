#ifndef LEMBALL_AI_OBJECTS_TIMEBONUS_H
#define LEMBALL_AI_OBJECTS_TIMEBONUS_H

#include "../../Common.h"
#include "Collectable.h" // complete type

// SIZE 0x13c
// VTABLE: LEMBALL 0x004955d0
class TimeBonus : public Collectable {
public:
	virtual int Collected(); // vtable+0x114
	virtual void SetSfx();   // vtable+0x110
	virtual ~TimeBonus();    // vtable+0x00
};

// SYNTHETIC: LEMBALL 0x00423010
// TimeBonus::`scalar deleting destructor'


#endif

