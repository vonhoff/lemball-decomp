#ifndef LEMBALL_VISOS_GRAPHICS_CGRAPHICBUTTON_H
#define LEMBALL_VISOS_GRAPHICS_CGRAPHICBUTTON_H

#include "CDepressedButton.h"
#include "Engine/Graphics/Primitives/CPushActive.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSSize.h"

class CPVGWnd;
class CResANIM;
class CRemap;
class CAnim;

// SIZE 0x130
// VTABLE: LEMBALL 0x00499748 CGWnd
// VTABLE: LEMBALL 0x00499728 CHotAreaHandler
class CGraphicButton : public CDepressedButton {
public:
	CGraphicButton(const CVSPoint& p_position,
				   CPVGWnd* p_parent,
				   unsigned long p_animId,
				   unsigned long p_alignmentFlags);
	virtual void DrawButton(); // vtable+0xbc
	virtual void OnDestroy();  // vtable+0x40
	virtual ~CGraphicButton(); // vtable+0x00
	void Initialise();
	void SetAnimID(unsigned long p_animId);

private:
	unsigned long m_animationId;    // 0x10c
	unsigned long m_alignmentFlags; // 0x110
	CVSSize m_graphicSize;          // 0x114
	CVSPoint m_graphicOffset;       // 0x118
	CResANIM* m_animation;          // 0x11c
	CRemap* m_remap;                // 0x120
	CAnim* m_primitive;             // 0x124
	CPushActive m_statRegion[1];    // 0x128
};

// SYNTHETIC: LEMBALL 0x004698c0
// CGraphicButton::`scalar deleting destructor'

#endif
