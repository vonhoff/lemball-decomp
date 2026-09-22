#ifndef LEMBALL_VISOS_GRAPHICS_CDEPRESSEDBUTTON_H
#define LEMBALL_VISOS_GRAPHICS_CDEPRESSEDBUTTON_H

#include "CPVButton.h" // complete type
class CPVGWnd;
class CVsRect;

// SIZE 0x10c
// VTABLE: LEMBALL 0x00497630 CGWnd
// VTABLE: LEMBALL 0x00497608 CHotAreaHandler
class CDepressedButton : public CPVButton {
public:
	CDepressedButton(CPVGWnd* p_parent) : CPVButton(p_parent)
	{
		m_state = 0;
		m_enabled = 0;
	}
	CDepressedButton(const CVsRect& p_rect, CPVGWnd* p_parent) : CPVButton(p_rect, p_parent)
	{
		m_state = 0;
		m_enabled = 0;
	}
	virtual void OnPaint(const CVsRect& p_rect); // vtable+0xa8
	virtual void _DrawButton();                  // vtable+0xb8
	virtual void OnReleased(int p_flags);        // vtable+0xc0
	virtual void OnPressed(int p_flags);         // vtable+0xc4
	virtual void OnEnterButton();                // vtable+0xc8
	virtual void OnExitButton();                 // vtable+0xcc
	friend class CPanelLemming;

protected:
	unsigned int m_enabled; // 0x104
	unsigned int m_state;   // 0x108
};

// SYNTHETIC: LEMBALL 0x00443ab0
// CDepressedButton::`scalar deleting destructor'

#endif
