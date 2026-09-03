#ifndef LEMBALL_AI_OBJECTS_KEY_H
#define LEMBALL_AI_OBJECTS_KEY_H

#include "../../Common.h"
#include "../Base/AiCoord.h"          // complete type
#include "../Base/BaseGlobalObject.h" // complete type

// SIZE 0x150
// VTABLE: LEMBALL 0x004943b0
class Key : public BaseGlobalObject {
public:
	inline Key(const AiCoord& p_position, eObjectType p_type) : BaseGlobalObject(p_position, p_type)
	{
		m_unk0x144 = DEBUG_SENTINEL;
		m_unk0x148 = DEBUG_SENTINEL;
		m_unk0x14c = DEBUG_SENTINEL;
	}
	virtual AiCoord ActivatePosition();          // vtable+0x2c
	virtual bool Activate(GameObject* p_object); // vtable+0x18
	virtual bool Process();                      // vtable+0x14
	virtual int Usage();                         // vtable+0x28
	virtual void DoActivate();                   // vtable+0x10c
	virtual ~Key();                              // vtable+0x00

private:
	int m_unk0x144;   // 0x144
	int m_unk0x148;   // 0x148
	int m_unk0x14c;   // 0x14c
};

// SYNTHETIC: LEMBALL 0x0041c600
// Key::`scalar deleting destructor'

#endif
