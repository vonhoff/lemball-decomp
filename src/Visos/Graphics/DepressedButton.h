#ifndef LEMBALL_VISOS_GRAPHICS_DEPRESSEDBUTTON_H
#define LEMBALL_VISOS_GRAPHICS_DEPRESSEDBUTTON_H

#include "../../Common.h"
#include "PvButton.h" // complete type

// SIZE 0x10c
class DepressedButton : public PvButton {
public:
	DepressedButton(const VsRect& p_rect, PvGWnd* p_parent) : PvButton(p_rect, p_parent)
	{
		m_lastDrawnDepressed = 0;
		m_depressed = 0;
	}
	virtual void OnPaint(const VsRect& p_rect); // vtable+0xa8
	virtual void _DrawButton();                 // vtable+0xb8
	virtual void OnReleased(int p_flags);       // vtable+0xc0
	virtual void OnPressed(int p_flags);        // vtable+0xc4
	virtual void OnEnterButton();               // vtable+0xc8
	virtual void OnExitButton();                // vtable+0xcc

private:
	unsigned int m_depressed;          // 0x104
	unsigned int m_lastDrawnDepressed; // 0x108
};

#endif
