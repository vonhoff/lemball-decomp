#ifndef LEMBALL_FRONTEND_SUPPORT_CENTRYHANDLER_H
#define LEMBALL_FRONTEND_SUPPORT_CENTRYHANDLER_H

#include "../../Visos/Graphics/CHotAreaHandler.h" // complete type

// SIZE 0x44
// VTABLE: LEMBALL 0x004986b0
class CEntryHandler : public CHotAreaHandler {
public:
	CEntryHandler();
	virtual void OnButtonDown(const CVsPoint& p_point, int p_flags); // vtable+0x04
	virtual void OnEnter();                                          // vtable+0x10
	virtual void OnExit();                                           // vtable+0x14
	void Reset();

	friend class CNetworkOptionsDrawer;

private:
	unsigned int m_hoverState;      // 0x38
	unsigned int m_pressed;         // 0x3c
	unsigned int m_activationState; // 0x40
};

// SYNTHETIC: LEMBALL 0x00455e10
// CEntryHandler::`vector deleting destructor'

// SYNTHETIC: LEMBALL 0x00455e90
// CEntryHandler::~CEntryHandler

#endif
