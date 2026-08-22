#ifndef LEMBALL_AI_OBJECTS_TRAPDOOR_H
#define LEMBALL_AI_OBJECTS_TRAPDOOR_H

#include "../../Common.h"
#include "../Base/BaseGlobalObject.h" // complete type

// SIZE 0x150
// VTABLE: LEMBALL 0x00493438
class TrapDoor : public BaseGlobalObject {
public:
	TrapDoor(AiCoord& p_arg0, unsigned char p_arg1);
	virtual bool Process();                         // vtable+0x14
	virtual int Usage();                            // vtable+0x28
	virtual void DoActivate();                      // vtable+0x10c
	virtual void GetViewData(ViewData& p_viewData); // vtable+0x0c
	virtual void Restart();                         // vtable+0x104
	virtual ~TrapDoor();                            // vtable+0x00

private:
	unsigned int m_mode;     // 0x144
	unsigned int m_active;   // 0x148
	unsigned int m_deadline; // 0x14c
};

#endif
