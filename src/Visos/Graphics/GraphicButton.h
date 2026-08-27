#ifndef LEMBALL_VISOS_GRAPHICS_GRAPHICBUTTON_H
#define LEMBALL_VISOS_GRAPHICS_GRAPHICBUTTON_H

#include "../../Common.h"
#include "PvButton.h" // complete type

// SIZE 0x130
// VTABLE: LEMBALL 0x00499748 primary
// VTABLE: LEMBALL 0x00499728 adjusted hot-area subobject at +0x90
class GraphicButton : public PvButton {
public:
	GraphicButton(const VsPoint& p_arg0, PvGWnd* p_arg1, unsigned long p_arg2, unsigned long p_arg3);
	virtual void _DrawButton();                        // vtable+0xb8
	virtual void DrawButton();                         // vtable+0xbc
	virtual void OnDestroy();                          // vtable+0x40
	virtual void OnPaint(const VsRect& p_rect);        // vtable+0xa8
	virtual ~GraphicButton();                          // vtable+0x00
	void Initialise();
	void SetAnimId(unsigned long p_animId);

	friend class ToggleButton;

private:
	unsigned int m_enabled;         // 0x104
	unsigned int m_state;           // 0x108
	unsigned int m_animationId;     // 0x10c
	unsigned int m_alignmentFlags;  // 0x110
	unsigned short m_graphicWidth;  // 0x114
	unsigned short m_graphicHeight; // 0x116
	short m_graphicOffsetX;         // 0x118
	short m_graphicOffsetY;         // 0x11a
	ResAnim* m_animation;           // 0x11c
	unsigned int m_frame;           // 0x120
	Primitive* m_primitive;         // 0x124
	undefined m_statRegion[8];      // 0x128
};

// SYNTHETIC: LEMBALL 0x004698c0
// GraphicButton::`scalar deleting destructor'


#endif

