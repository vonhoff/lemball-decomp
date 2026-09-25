#ifndef LEMBALL_AI_OBJECTS_CBALLOON_H
#define LEMBALL_AI_OBJECTS_CBALLOON_H

#include "../Base/AiCoord.h"
#include "../Base/CBaseGlobalObject.h"
#include "AI/Base/ObjectTypes.h"

// SIZE 0x150
// VTABLE: LEMBALL 0x004944f8
class CBalloon : public CBaseGlobalObject {
public:
	inline CBalloon(const AiCoord& p_position, eObjectType p_type) : CBaseGlobalObject(p_position, p_type) {}
	virtual AiCoord ActivatePosition();           // vtable+0x2c
	virtual bool Activate(CGameObject* p_object); // vtable+0x18
	virtual bool Process();                       // vtable+0x14
	virtual int Usage();                          // vtable+0x28
	virtual void DoActivate();                    // vtable+0x10c
	virtual void Restart();                       // vtable+0x104

private:
	AiCoord m_unk0x144; // 0x144
};

// SYNTHETIC: LEMBALL 0x0041c640
// CBalloon::`scalar deleting destructor'

#endif
