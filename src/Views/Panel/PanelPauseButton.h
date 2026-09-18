#ifndef LEMBALL_VIEWS_PANEL_PANELPAUSEBUTTON_H
#define LEMBALL_VIEWS_PANEL_PANELPAUSEBUTTON_H

#include "../../Visos/Graphics/ToggleButton.h" // complete type

class GWnd;
class HotAreaHandler;
class Panel;
class PvGWnd;
class VsRect;
struct VsPoint;
// SIZE 0x13c
// VTABLE: LEMBALL 0x004972b8 GWnd
// VTABLE: LEMBALL 0x00497290 HotAreaHandler
class PanelPauseButton : public ToggleButton {
public:
	PanelPauseButton(Panel* p_arg0, const VsPoint& p_arg1, PvGWnd* p_arg2, unsigned long p_arg3, unsigned long p_arg4);
	virtual void OnPaint(const VsRect& p_rect);                           // vtable+0xa8
	virtual void DrawButton();                                            // vtable+0xbc
	virtual void OnExternalButtonUp(const VsPoint& p_point, int p_flags); // vtable+0x0c
	virtual void OnInside(const VsPoint& p_point);                        // vtable+0x18
	virtual void OnPressed(int p_flags);                                  // vtable+0xc4
	virtual void OnReleased(int p_flags);                                 // vtable+0xc0

private:
	Panel* m_panel;               // 0x134
	unsigned int m_pressedInside; // 0x138
};

// SYNTHETIC: LEMBALL 0x004438f0
// PanelPauseButton::`scalar deleting destructor'

#endif
