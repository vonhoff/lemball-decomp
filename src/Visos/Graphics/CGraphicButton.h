#ifndef LEMBALL_VISOS_GRAPHICS_CGRAPHICBUTTON_H
#define LEMBALL_VISOS_GRAPHICS_CGRAPHICBUTTON_H

#include "CDepressedButton.h" // complete type
#include "CPushActive.h"      // complete type

struct CVsPoint;
class CPVGWnd;
class CResANIM;
class CPrimitive;

// SIZE 0x130
// VTABLE: LEMBALL 0x00499748 CGWnd
// VTABLE: LEMBALL 0x00499728 CHotAreaHandler
class CGraphicButton : public CDepressedButton {
public:
	CGraphicButton(const CVsPoint& p_position,
				   CPVGWnd* p_parent,
				   unsigned long p_animId,
				   unsigned long p_alignmentFlags);
	virtual void DrawButton(); // vtable+0xbc
	virtual void OnDestroy();  // vtable+0x40
	virtual ~CGraphicButton(); // vtable+0x00
	void Initialise();
	void SetAnimID(unsigned long p_animId);

	friend class CToggleButton;
	friend class CGunButton;
	friend class CGunButtons;
	friend class CHiliteButtons;
	friend class CPanel;
	friend class CPanelPauseButton;

private:
	unsigned int m_animationId;     // 0x10c
	unsigned int m_alignmentFlags;  // 0x110
	unsigned short m_graphicWidth;  // 0x114
	unsigned short m_graphicHeight; // 0x116
	short m_graphicOffsetX;         // 0x118
	short m_graphicOffsetY;         // 0x11a
	CResANIM* m_animation;          // 0x11c
	unsigned int m_frame;           // 0x120
	CPrimitive* m_primitive;        // 0x124
	CPushActive m_statRegion[1];    // 0x128
};

// SYNTHETIC: LEMBALL 0x004698c0
// CGraphicButton::`scalar deleting destructor'

#endif
