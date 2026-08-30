#ifndef LEMBALL_AI_OBJECTS_AMMO_H
#define LEMBALL_AI_OBJECTS_AMMO_H

#include "../../Common.h"
#include "../Base/AiCoord.h"          // complete type
#include "../Base/BaseGlobalObject.h" // complete type

// MINIMUM SIZE 0x146
// VTABLE: LEMBALL 0x00494268
class Ammo : public BaseGlobalObject {
public:
	virtual AiCoord ActivatePosition();          // vtable+0x2c
	virtual bool Activate(GameObject* p_object); // vtable+0x18
	virtual bool Process();                      // vtable+0x14
	virtual int Usage();                         // vtable+0x28
	virtual void DoActivate();                   // vtable+0x10c
	virtual void Restart();                      // vtable+0x104
	virtual ~Ammo();                             // vtable+0x00

private:
	unsigned short m_ammo; // 0x144
};

// SYNTHETIC: LEMBALL 0x0041c440
// Ammo::`scalar deleting destructor'

#endif
