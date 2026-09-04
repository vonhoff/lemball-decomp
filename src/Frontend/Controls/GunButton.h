#ifndef LEMBALL_FRONTEND_CONTROLS_GUNBUTTON_H
#define LEMBALL_FRONTEND_CONTROLS_GUNBUTTON_H

#include "../../Common.h"
#include "../../Visos/Graphics/GraphicButton.h" // complete type

// SIZE 0x130
// VTABLE: LEMBALL 0x00497d30 GWnd
// VTABLE: LEMBALL 0x00497d08 HotAreaHandler
class GunButton : public GraphicButton {
public:
	GunButton(const VsPoint& p_position, PvGWnd* p_parent, unsigned long p_animId, unsigned long p_flags);
	virtual void OnPressed(int p_flags);  // vtable+0xc4
	virtual void OnReleased(int p_flags); // vtable+0xc0
	virtual ~GunButton();                 // vtable+0x00
};

// SYNTHETIC: LEMBALL 0x0044e650
// GunButton::`scalar deleting destructor'

#endif
