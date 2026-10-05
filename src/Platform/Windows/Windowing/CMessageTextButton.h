#ifndef LEMBALL_VISOS_TARGET_UI_CMESSAGETEXTBUTTON_H
#define LEMBALL_VISOS_TARGET_UI_CMESSAGETEXTBUTTON_H

#include "CTextButton.h"

// MINIMUM SIZE 0x14c
// VTABLE: LEMBALL 0x00499c18 CGWnd
// VTABLE: LEMBALL 0x00499bf8 CHotAreaHandler
class CMessageTextButton : public CTextButton {
public:
	CMessageTextButton(unsigned int p_controlMessage,
					   const CVSRect& p_rect,
					   CPVGWnd* p_parent,
					   unsigned int p_fontResourceId,
					   unsigned int p_alignmentFlags);
	void Initialize();
};

// SYNTHETIC: LEMBALL 0x00469c30
// CMessageTextButton::`scalar deleting destructor'

#endif
