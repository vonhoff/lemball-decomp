#ifndef LEMBALL_VISOS_GRAPHICS_DEPRESSEDBUTTON_H
#define LEMBALL_VISOS_GRAPHICS_DEPRESSEDBUTTON_H

#include "../../Common.h"
#include "PvButton.h" // complete type

// SIZE 0x10c
// VTABLE: LEMBALL 0x00497630 GWnd
// VTABLE: LEMBALL 0x00497608 HotAreaHandler
class DepressedButton : public PvButton {
public:
	DepressedButton(PvGWnd* p_parent) : PvButton(p_parent)
	{
		m_state = 0;
		m_enabled = 0;
	}
	DepressedButton(const VsRect& p_rect, PvGWnd* p_parent) : PvButton(p_rect, p_parent)
	{
		m_state = 0;
		m_enabled = 0;
	}
	virtual void OnPaint(const VsRect& p_rect); // vtable+0xa8
	virtual void InternalDrawButton();          // vtable+0xb8
	virtual void OnReleased(int p_flags);       // vtable+0xc0
	virtual void OnPressed(int p_flags);        // vtable+0xc4
	virtual void OnEnterButton();               // vtable+0xc8
	virtual void OnExitButton();                // vtable+0xcc
	friend class PanelLemming;

protected:
	unsigned int m_enabled; // 0x104
	unsigned int m_state;   // 0x108
};

#endif
