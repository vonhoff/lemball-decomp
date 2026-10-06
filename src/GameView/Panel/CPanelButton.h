#ifndef LEMBALL_VIEWS_PANEL_CPANELBUTTON_H
#define LEMBALL_VIEWS_PANEL_CPANELBUTTON_H

#include "Gameplay/Objects/ObjectTypes.h"
#include "../../Engine/Animation/CAnim.h"
#include "Engine/Math/CVSRect.h"
#include "Platform/Windows/Windowing/CDepressedButton.h"
#include "Engine/Graphics/Primitives/CSolidRect.h"

class CPanelLemming;
class CPVGWnd;
// SIZE 0x1b8
// VTABLE: LEMBALL 0x00497508 CGWnd
// VTABLE: LEMBALL 0x004974e0 CHotAreaHandler
class CPanelButton : public CDepressedButton {
public:
	CPanelButton(CPanelLemming* p_lemming, const CVSRect& p_rect, CPVGWnd* p_parent);
	virtual void OnPaint(const CVSRect& p_rect);                           // vtable+0xa8
	virtual void DrawButton();                                             // vtable+0xbc
	virtual void OnEnterButton();                                          // vtable+0xc8
	virtual void OnExitButton();                                           // vtable+0xcc
	virtual void OnExternalButtonUp(const CVSPoint& p_point, eMouseButtonIndex p_flags); // vtable+0x0c
	virtual void OnInside(const CVSPoint& p_point);                        // vtable+0x18
	virtual void OnPressed(eMouseButtonIndex p_flags);                     // vtable+0xc4
	virtual void OnReleased(int p_flags);                                  // vtable+0xc0
	virtual ~CPanelButton();                                               // vtable+0x00

private:
	unsigned int m_pressedInside;   // 0x10c
	CPanelLemming* m_lemming;       // 0x110
	CSolidRect m_statusLine[1];     // 0x114
	CSolidRect m_inventoryLines[3]; // 0x124
	CVSRect m_statusRect;           // 0x154
	CVSRect m_inventoryRect;        // 0x15c
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
