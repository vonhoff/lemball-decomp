#ifndef LEMBALL_VIEWS_PANEL_PANELPAUSEBUTTONHOTAREASUBOBJECT_H
#define LEMBALL_VIEWS_PANEL_PANELPAUSEBUTTONHOTAREASUBOBJECT_H

#include "../../Common.h"

// SIZE 0xac
class PanelPauseButtonHotAreaSubobject {
public:
	// No functions mapped yet.

private:
	unsigned int m_enabled;         // 0x74
	unsigned int m_state;           // 0x78
	unsigned int m_animId;          // 0x7c
	unsigned int m_alignmentFlags;  // 0x80
	unsigned short m_graphicWidth;  // 0x84
	unsigned short m_graphicHeight; // 0x86
	short m_graphicOffsetX;         // 0x88
	short m_graphicOffsetY;         // 0x8a
	ResAnim* m_animation;           // 0x8c
	unsigned int m_frame;           // 0x90
	void* m_primitive;              // 0x94
	unsigned int m_paused;          // 0xa0
	Panel* m_panel;                 // 0xa4
	unsigned int m_pressedInside;   // 0xa8
};

#endif
