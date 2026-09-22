#ifndef LEMBALL_VIEWS_PANEL_CPANELPAUSEBUTTON_H
#define LEMBALL_VIEWS_PANEL_CPANELPAUSEBUTTON_H

#include "../../Visos/Graphics/CToggleButton.h" // complete type

class CPanel;
class CPVGWnd;
struct CVsPoint;
// SIZE 0x13c
// VTABLE: LEMBALL 0x004972b8 CGWnd
// VTABLE: LEMBALL 0x00497290 CHotAreaHandler
class CPanelPauseButton : public CToggleButton {
public:
	CPanelPauseButton(CPanel* p_arg0,
					  const CVsPoint& p_arg1,
					  CPVGWnd* p_arg2,
					  unsigned long p_arg3,
					  unsigned long p_arg4);
	virtual void OnPaint(const CVsRect& p_rect);                           // vtable+0xa8
	virtual void DrawButton();                                             // vtable+0xbc
	virtual void OnExternalButtonUp(const CVsPoint& p_point, int p_flags); // vtable+0x0c
	virtual void OnInside(const CVsPoint& p_point);                        // vtable+0x18
	virtual void OnPressed(int p_flags);                                   // vtable+0xc4
	virtual void OnReleased(int p_flags);                                  // vtable+0xc0

private:
	CPanel* m_panel;              // 0x134
	unsigned int m_pressedInside; // 0x138
};

// SYNTHETIC: LEMBALL 0x004438f0
// CPanelPauseButton::`scalar deleting destructor'

#endif
