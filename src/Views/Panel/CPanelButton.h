#ifndef LEMBALL_VIEWS_PANEL_CPANELBUTTON_H
#define LEMBALL_VIEWS_PANEL_CPANELBUTTON_H

#include "../../AI/Base/ObjectTypes.h"
#include "../../Visos/Animation/CAnim.h"           // complete type
#include "../../Visos/Foundation/CVsRect.h"        // complete type
#include "../../Visos/Graphics/CDepressedButton.h" // complete type
#include "../../Visos/Graphics/CLine.h"            // complete type

class CPanelLemming;
class CPvGWnd;
// SIZE 0x1b8
// VTABLE: LEMBALL 0x00497508 CGWnd
// VTABLE: LEMBALL 0x004974e0 CHotAreaHandler
class CPanelButton : public CDepressedButton {
public:
	CPanelButton(CPanelLemming* p_arg0, const CVsRect& p_arg1, CPvGWnd* p_arg2);
	virtual void OnPaint(const CVsRect& p_rect);                           // vtable+0xa8
	virtual void DrawButton();                                             // vtable+0xbc
	virtual void OnEnterButton();                                          // vtable+0xc8
	virtual void OnExitButton();                                           // vtable+0xcc
	virtual void OnExternalButtonUp(const CVsPoint& p_point, int p_flags); // vtable+0x0c
	virtual void OnInside(const CVsPoint& p_point);                        // vtable+0x18
	virtual void OnPressed(int p_flags);                                   // vtable+0xc4
	virtual void OnReleased(int p_flags);                                  // vtable+0xc0
	virtual ~CPanelButton();                                               // vtable+0x00

private:
	unsigned int m_pressedInside;   // 0x10c
	CPanelLemming* m_lemming;       // 0x110
	CLine m_statusLine[1];          // 0x114
	CLine m_inventoryLines[3];      // 0x124
	CVsRect m_statusRect;           // 0x154
	CVsRect m_inventoryRect;        // 0x15c
	unsigned int m_lastAmmo;        // 0x164
	unsigned int m_unavailable;     // 0x168
	unsigned int m_alternatePlayer; // 0x16c
	eObjectType m_lastBalloon;      // 0x170
	unsigned int m_inventoryCount;  // 0x174
	CAnim m_statusAnim[1];          // 0x178
	CAnim m_inventoryAnim[1];       // 0x198
};

// SYNTHETIC: LEMBALL 0x00443950
// CPanelButton::`scalar deleting destructor'

#endif
