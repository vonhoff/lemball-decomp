#ifndef LEMBALL_VISOS_GRAPHICS_CTOGGLEBUTTON_H
#define LEMBALL_VISOS_GRAPHICS_CTOGGLEBUTTON_H

#include "CGraphicButton.h"
#include "Engine/Queues/Message.h"

class CPVGWnd;
struct CVSPoint;

// SIZE 0x134
// VTABLE: LEMBALL 0x004973e0 CGWnd
// VTABLE: LEMBALL 0x004973b8 CHotAreaHandler
class CToggleButton : public CGraphicButton {
public:
	CToggleButton(const CVSPoint& p_position, CPVGWnd* p_parent, unsigned long p_animId, unsigned long p_flags)
		: CGraphicButton(p_position, p_parent, p_animId, p_flags)
	{
		m_toggled = 0;
	}
	virtual void OnEnterButton();                       // vtable+0xc8
	virtual void OnExitButton();                        // vtable+0xcc
	virtual void OnPressed(eMouseButtonIndex p_flags);  // vtable+0xc4
	virtual void OnReleased(eMouseButtonIndex p_flags); // vtable+0xc0
	friend class CPanel;
	friend class CPanelPauseButton;

private:
	unsigned int m_toggled; // 0x130
};

// SYNTHETIC: LEMBALL 0x004438b0
// CToggleButton::`scalar deleting destructor'

#endif
