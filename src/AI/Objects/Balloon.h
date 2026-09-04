#ifndef LEMBALL_AI_OBJECTS_BALLOON_H
#define LEMBALL_AI_OBJECTS_BALLOON_H

#include "../../Common.h"
#include "../Base/AiCoord.h"          // complete type
#include "../Base/BaseGlobalObject.h" // complete type

// SIZE 0x150
// VTABLE: LEMBALL 0x004944f8
class Balloon : public BaseGlobalObject {
public:
	inline Balloon(const AiCoord& p_position, eObjectType p_type) : BaseGlobalObject(p_position, p_type) {}
	virtual AiCoord ActivatePosition();          // vtable+0x2c
	virtual bool Activate(GameObject* p_object); // vtable+0x18
	virtual bool Process();                      // vtable+0x14
	virtual int Usage();                         // vtable+0x28
	virtual void DoActivate();                   // vtable+0x10c
	virtual void Restart();                      // vtable+0x104
	virtual ~Balloon();                          // vtable+0x00

private:
	AiCoord m_unk0x144; // 0x144
};

// SYNTHETIC: LEMBALL 0x0041c640
// Balloon::`scalar deleting destructor'

#endif
