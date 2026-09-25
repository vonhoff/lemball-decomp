#ifndef LEMBALL_AI_OBJECTS_CCRATE_H
#define LEMBALL_AI_OBJECTS_CCRATE_H

#include "../Base/AiCoord.h"
#include "../Base/CBaseGlobalObject.h"
#include "AI/Base/ObjectTypes.h"

class CGlobalGameObject;
// MINIMUM SIZE 0x150
// VTABLE: LEMBALL 0x004948d0
class CCrate : public CBaseGlobalObject {
public:
	CCrate(const AiCoord& p_position, CGlobalGameObject* p_contents, unsigned short p_contentsId);
	virtual AiCoord ActivatePosition();           // vtable+0x2c
	virtual bool Activate(CGameObject* p_object); // vtable+0x18
	virtual bool Process();                       // vtable+0x14
	virtual int Usage();                          // vtable+0x28
	virtual void DoActivate();                    // vtable+0x10c
	virtual void Restart();                       // vtable+0x104
	virtual ~CCrate();                            // vtable+0x00
	void TriggerContents();

	friend class CObjectManager;

private:
	unsigned short m_contentsId;   // 0x144
	CGlobalGameObject* m_contents; // 0x148
	eObjectType m_contentsType;    // 0x14c
};

// SYNTHETIC: LEMBALL 0x0041c540
// CCrate::`scalar deleting destructor'

#endif
