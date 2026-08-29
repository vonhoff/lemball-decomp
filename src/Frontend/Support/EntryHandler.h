#ifndef LEMBALL_FRONTEND_SUPPORT_ENTRYHANDLER_H
#define LEMBALL_FRONTEND_SUPPORT_ENTRYHANDLER_H

#include "../../Common.h"
#include "../../Visos/Graphics/HotAreaHandler.h" // complete type

// SIZE 0x44
// VTABLE: LEMBALL 0x004986b0
class EntryHandler : public HotAreaHandler {
public:
	EntryHandler();
	virtual ~EntryHandler();                                        // vtable+0x00
	virtual void OnButtonDown(const VsPoint& p_point, int p_flags);   // vtable+0x04
	virtual void OnEnter();                                                 // vtable+0x10
	virtual void OnExit();                                                  // vtable+0x14
	void Reset();

	friend class NetworkOptionsDrawer;

private:
	unsigned int m_hoverState;      // 0x38
	unsigned int m_pressed;         // 0x3c
	unsigned int m_activationState; // 0x40
};

// SYNTHETIC: LEMBALL 0x00455e10
// EntryHandler::`scalar deleting destructor'


#endif

