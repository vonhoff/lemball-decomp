#ifndef LEMBALL_AI_OBJECTS_CSHEEP_H
#define LEMBALL_AI_OBJECTS_CSHEEP_H

#include "../Base/CGameObject.h"

class CAI;
// SIZE 0x124
// VTABLE: LEMBALL 0x00494f80
class CSheep : public CGameObject {
public:
	CSheep(CAI* p_ai, int p_x, int p_y, int p_z, int p_facingDirection);
	virtual bool Process(); // vtable+0x14
	virtual void HitBall(); // vtable+0x58
	virtual void HitMine(); // vtable+0x64
	virtual void Restart(); // vtable+0x104
};

// SYNTHETIC: LEMBALL 0x0041fb70
// CSheep::`scalar deleting destructor'

#endif
