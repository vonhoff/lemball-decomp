#ifndef LEMBALL_AI_OBJECTS_CAMMO_H
#define LEMBALL_AI_OBJECTS_CAMMO_H

#include "../Base/AiCoord.h"
#include "../Base/CBaseGlobalObject.h"
#include "AI/Base/ObjectTypes.h"

// MINIMUM SIZE 0x146
// VTABLE: LEMBALL 0x00494268
class CAmmo : public CBaseGlobalObject {
public:
	inline CAmmo(const AiCoord& p_position) : CBaseGlobalObject(p_position, OBJECT_AMMO) {}
	virtual AiCoord ActivatePosition();           // vtable+0x2c
	virtual bool Activate(CGameObject* p_object); // vtable+0x18
	virtual bool Process();                       // vtable+0x14
	virtual int Usage();                          // vtable+0x28
	virtual void DoActivate();                    // vtable+0x10c
	virtual void Restart();                       // vtable+0x104

	friend class CObjectManager;

private:
	unsigned short m_ammo; // 0x144
};

// SYNTHETIC: LEMBALL 0x0041c440
// CAmmo::`scalar deleting destructor'

#endif
