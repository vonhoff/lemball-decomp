#ifndef LEMBALL_AI_OBJECTS_BONUS_H
#define LEMBALL_AI_OBJECTS_BONUS_H

#include "../../Common.h"
#include "Collectable.h" // complete type

// SIZE 0x13c
// VTABLE: LEMBALL 0x00495870
class Bonus : public Collectable {
public:
	virtual int Collected(); // vtable+0x114
	virtual void SetSfx();   // vtable+0x110
	virtual ~Bonus();        // vtable+0x00
};

// SYNTHETIC: LEMBALL 0x00422fe0
// Bonus::`scalar deleting destructor'

#endif
