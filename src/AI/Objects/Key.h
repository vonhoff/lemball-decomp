#ifndef LEMBALL_AI_OBJECTS_KEY_H
#define LEMBALL_AI_OBJECTS_KEY_H

#include "../../Common.h"
#include "../Base/AiCoord.h"          // complete type
#include "../Base/BaseGlobalObject.h" // complete type

// MINIMUM SIZE 0x144
// VTABLE: LEMBALL 0x004943b0
class Key : public BaseGlobalObject {
public:
	virtual AiCoord ActivatePosition();          // vtable+0x2c
	virtual bool Activate(GameObject* p_object); // vtable+0x18
	virtual bool Process();                      // vtable+0x14
	virtual int Usage();                         // vtable+0x28
	virtual void DoActivate();                   // vtable+0x10c
	virtual ~Key();                              // vtable+0x00
};

// SYNTHETIC: LEMBALL 0x0041c600
// Key::`scalar deleting destructor'


#endif

