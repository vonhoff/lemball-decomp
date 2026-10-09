#ifndef LEMBALL_AI_OBJECTS_CTOWER_H
#define LEMBALL_AI_OBJECTS_CTOWER_H

#include "Gameplay/Objects/CBaseGlobalObject.h"
#include "Gameplay/Objects/ObjectTypes.h"

class AICOORD;

// MINIMUM SIZE 0x144
// VTABLE: LEMBALL 0x00494788
class CTower : public CBaseGlobalObject {
public:
	CTower(const AICOORD& p_position) : CBaseGlobalObject(p_position, OBJECT_TOWER) {}
	virtual AICOORD ActivatePosition();           // vtable+0x2c
	virtual bool Activate(CGameObject* p_object); // vtable+0x18
	virtual bool Process();                       // vtable+0x14
	virtual int Usage();                          // vtable+0x28
	virtual void DoActivate();                    // vtable+0x10c
};

// SYNTHETIC: LEMBALL 0x0041c5c0
// CTower::`scalar deleting destructor'

#endif
