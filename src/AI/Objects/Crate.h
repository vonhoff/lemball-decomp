#ifndef LEMBALL_AI_OBJECTS_CRATE_H
#define LEMBALL_AI_OBJECTS_CRATE_H

#include "../../Common.h"
#include "../Base/AiCoord.h"          // complete type
#include "../Base/BaseGlobalObject.h" // complete type

// MINIMUM SIZE 0x150
// VTABLE: LEMBALL 0x004948d0
class Crate : public BaseGlobalObject {
public:
	virtual AiCoord ActivatePosition();          // vtable+0x2c
	virtual bool Activate(GameObject* p_object); // vtable+0x18
	virtual bool Process();                      // vtable+0x14
	virtual int Usage();                         // vtable+0x28
	virtual void DoActivate();                   // vtable+0x10c
	virtual void Restart();                      // vtable+0x104
	virtual ~Crate();                            // vtable+0x00
	void TriggerContents();

private:
	unsigned short m_contentsId;  // 0x144
	GlobalGameObject* m_contents; // 0x148
	eObjectType m_contentsType;   // 0x14c
};

// SYNTHETIC: LEMBALL 0x0041c540
// Crate::`scalar deleting destructor'

#endif
