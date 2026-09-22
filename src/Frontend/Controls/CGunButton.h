#ifndef LEMBALL_FRONTEND_CONTROLS_CGUNBUTTON_H
#define LEMBALL_FRONTEND_CONTROLS_CGUNBUTTON_H

#include "../../Visos/Graphics/CGraphicButton.h" // complete type

class CPVGWnd;
struct CVsPoint;
// SIZE 0x130
// VTABLE: LEMBALL 0x00497d30 CGWnd
// VTABLE: LEMBALL 0x00497d08 CHotAreaHandler
class CGunButton : public CGraphicButton {
public:
	CGunButton(const CVsPoint& p_position, CPVGWnd* p_parent, unsigned long p_animId, unsigned long p_flags)
		: CGraphicButton(p_position, p_parent, p_animId, p_flags)
	{
	}
	virtual void OnPressed(int p_flags);  // vtable+0xc4
	virtual void OnReleased(int p_flags); // vtable+0xc0
};

// SYNTHETIC: LEMBALL 0x0044e650
// CGunButton::`scalar deleting destructor'

#endif
