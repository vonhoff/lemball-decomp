#ifndef LEMBALL_VISOS_GRAPHICS_TOGGLEBUTTON_H
#define LEMBALL_VISOS_GRAPHICS_TOGGLEBUTTON_H

#include "../../Common.h"
#include "GraphicButton.h" // complete type

// SIZE 0x134
// VTABLE: LEMBALL 0x004973e0 primary
// VTABLE: LEMBALL 0x004973b8 adjusted hot-area subobject at +0x90
class ToggleButton : public GraphicButton {
public:
	virtual void OnEnterButton();         // vtable+0xc8
	virtual void OnExitButton();          // vtable+0xcc
	virtual void OnPressed(int p_flags);  // vtable+0xc4
	virtual void OnReleased(int p_flags); // vtable+0xc0
	virtual ~ToggleButton();              // vtable+0x00

private:
	unsigned int m_toggled; // 0x130
};

// SYNTHETIC: LEMBALL 0x004438b0
// ToggleButton::`scalar deleting destructor'


#endif

