#ifndef LEMBALL_VISOS_TARGET_UI_CSTATETEXTBUTTON_H
#define LEMBALL_VISOS_TARGET_UI_CSTATETEXTBUTTON_H

#include "CTextButton.h"

class CPVGWnd;
class CVSRect;

// MINIMUM SIZE 0x158
// VTABLE: LEMBALL 0x00499b28 CGWnd
// VTABLE: LEMBALL 0x00499b08 CHotAreaHandler
class CStateTextButton : public CTextButton {
public:
	CStateTextButton(unsigned int p_controlMessage,
					 const CVSRect& p_rect,
					 CPVGWnd* p_parent,
					 unsigned int p_fontResourceId,
					 unsigned int p_alignmentFlags);
	void InitializeState();

private:
	unsigned int m_state; // 0x14c
	char* m_normalText;   // 0x150
	char* m_activeText;   // 0x154
};

// SYNTHETIC: LEMBALL 0x00469c00
// CStateTextButton::`scalar deleting destructor'

#endif
