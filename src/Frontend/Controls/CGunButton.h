#ifndef LEMBALL_FRONTEND_CONTROLS_CGUNBUTTON_H
#define LEMBALL_FRONTEND_CONTROLS_CGUNBUTTON_H

#include "Engine/Queues/Message.h"
#include "Platform/Windows/Windowing/CGraphicButton.h"

class CPVGWnd;
struct CVSPoint;

// SIZE 0x130
// VTABLE: LEMBALL 0x00497d30 CGWnd
// VTABLE: LEMBALL 0x00497d08 CHotAreaHandler
class CGunButton : public CGraphicButton {
public:
	CGunButton(const CVSPoint& p_position, CPVGWnd* p_parent, unsigned long p_animId, unsigned long p_flags)
		: CGraphicButton(p_position, p_parent, p_animId, p_flags)
	{
	}
	virtual void OnPressed(eMouseButtonIndex p_flags);  // vtable+0xc4
	virtual void OnReleased(eMouseButtonIndex p_flags); // vtable+0xc0
};

// SYNTHETIC: LEMBALL 0x0044e650
// CGunButton::`scalar deleting destructor'

#endif
