#ifndef LEMBALL_VIEWS_PANEL_PANELBUTTON_H
#define LEMBALL_VIEWS_PANEL_PANELBUTTON_H

#include "../../Common.h"
#include "../../Visos/Animation/Anim.h"           // complete type
#include "../../Visos/Foundation/VsRect.h"        // complete type
#include "../../Visos/Graphics/DepressedButton.h" // complete type
#include "../../Visos/Graphics/Line.h"            // complete type

// SIZE 0x1b8
// VTABLE: LEMBALL 0x00497508 primary
// VTABLE: LEMBALL 0x004974e0 adjusted hot-area subobject at +0x90
class PanelButton : public DepressedButton {
public:
	PanelButton(PanelLemming* p_arg0, const VsRect& p_arg1, PvGWnd* p_arg2);
	virtual void OnPaint(const VsRect& p_rect);                   // vtable+0xa8
	virtual void DrawButton();                                            // vtable+0xbc
	virtual void OnEnterButton();                                         // vtable+0xc8
	virtual void OnExitButton();                                          // vtable+0xcc
	virtual void OnExternalButtonUp(const VsPoint& p_point, int p_flags); // vtable+0x0c
	virtual void OnInside(const VsPoint& p_point);                        // vtable+0x18
	virtual void OnPressed(int p_flags);                                  // vtable+0xc4
	virtual void OnReleased(int p_flags);                                 // vtable+0xc0
	virtual ~PanelButton();                                               // vtable+0x00

private:
	unsigned int m_pressedInside;   // 0x10c
	PanelLemming* m_lemming;        // 0x110
	Line m_statusLine;              // 0x114
	Line m_inventoryLines[3];       // 0x124
	VsRect m_statusRect;            // 0x154
	VsRect m_inventoryRect;         // 0x15c
	unsigned int m_lastAmmo;        // 0x164
	unsigned int m_unavailable;     // 0x168
	unsigned int m_alternatePlayer; // 0x16c
	eObjectType m_lastBalloon;      // 0x170
	unsigned int m_inventoryCount;  // 0x174
	Anim m_statusAnim;              // 0x178
	Anim m_inventoryAnim;           // 0x198
};

#endif
