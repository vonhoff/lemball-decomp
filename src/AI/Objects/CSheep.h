#ifndef LEMBALL_AI_OBJECTS_CSHEEP_H
#define LEMBALL_AI_OBJECTS_CSHEEP_H

#include "../Base/CGameObject.h" // complete type

class CAI;
// SIZE 0x124
// VTABLE: LEMBALL 0x00494f80
class CSheep : public CGameObject {
public:
	CSheep(CAI* p_arg0, int p_arg1, int p_arg2, int p_arg3, int p_arg4);
	virtual bool Process(); // vtable+0x14
	virtual void HitBall(); // vtable+0x58
	virtual void HitMine(); // vtable+0x64
	virtual void Restart(); // vtable+0x104
};

// SYNTHETIC: LEMBALL 0x0041fb70
// CSheep::`scalar deleting destructor'

#endif
