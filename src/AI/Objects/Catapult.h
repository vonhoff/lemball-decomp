#ifndef LEMBALL_AI_OBJECTS_CATAPULT_H
#define LEMBALL_AI_OBJECTS_CATAPULT_H

#include "../../Common.h"
#include "../Base/AiCoord.h"          // complete type
#include "../Base/BaseGlobalObject.h" // complete type

// MINIMUM SIZE 0x144
// VTABLE: LEMBALL 0x00494640
class Catapult : public BaseGlobalObject {
public:
	inline Catapult(const AiCoord& p_position) : BaseGlobalObject(p_position, 4)
	{
	}
	virtual AiCoord ActivatePosition();          // vtable+0x2c
	virtual bool Activate(GameObject* p_object); // vtable+0x18
	virtual bool Process();                      // vtable+0x14
	virtual int Usage();                         // vtable+0x28
	virtual void DoActivate();                   // vtable+0x10c
	virtual void Restart();                      // vtable+0x104
	virtual ~Catapult();                         // vtable+0x00
};

// SYNTHETIC: LEMBALL 0x0041c400
// Catapult::`scalar deleting destructor'

#endif
