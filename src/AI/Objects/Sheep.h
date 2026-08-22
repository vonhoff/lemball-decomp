#ifndef LEMBALL_AI_OBJECTS_SHEEP_H
#define LEMBALL_AI_OBJECTS_SHEEP_H

#include "../../Common.h"
#include "../Base/GameObject.h" // complete type

// SIZE 0x124
// VTABLE: LEMBALL 0x00494f80
class Sheep : public GameObject {
public:
	Sheep(Ai* p_arg0, int p_arg1, int p_arg2, int p_arg3, int p_arg4);
	virtual bool Process(); // vtable+0x14
	virtual void HitBall(); // vtable+0x58
	virtual void HitMine(); // vtable+0x64
	virtual void Restart(); // vtable+0x104
	virtual ~Sheep();       // vtable+0x00
};

#endif
