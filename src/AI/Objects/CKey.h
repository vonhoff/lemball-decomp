#ifndef LEMBALL_AI_OBJECTS_CKEY_H
#define LEMBALL_AI_OBJECTS_CKEY_H

#include "../Base/AiCoord.h"
#include "../Base/CBaseGlobalObject.h"
#include "AI/Base/ObjectTypes.h"

// SIZE 0x150
// VTABLE: LEMBALL 0x004943b0
class CKey : public CBaseGlobalObject {
public:
	inline CKey(const AiCoord& p_position, eObjectType p_type) : CBaseGlobalObject(p_position, p_type) {}
	virtual AiCoord ActivatePosition();           // vtable+0x2c
	virtual bool Activate(CGameObject* p_object); // vtable+0x18
	virtual bool Process();                       // vtable+0x14
	virtual int Usage();                          // vtable+0x28
	virtual void DoActivate();                    // vtable+0x10c

private:
	AiCoord m_unk0x144; // 0x144
};

// SYNTHETIC: LEMBALL 0x0041c600
// CKey::`scalar deleting destructor'

#endif
