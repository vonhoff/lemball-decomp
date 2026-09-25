#ifndef LEMBALL_VISOS_TARGET_UI_CRESOURCEBUTTON_H
#define LEMBALL_VISOS_TARGET_UI_CRESOURCEBUTTON_H

#include "CFramedButton.h"

class CGWnd;
class CHotAreaHandler;
class CVsRect;
// MINIMUM SIZE 0x11c
// VTABLE: LEMBALL 0x00499928 CGWnd
// VTABLE: LEMBALL 0x00499908 CHotAreaHandler
class CResourceButton : public CFramedButton {
public:
	virtual ~CResourceButton();
	virtual void OnPaint(const CVsRect& p_rect);
	virtual void OnDestroy();

private:
	void* m_resourceInterface; // 0x118
};

// SYNTHETIC: LEMBALL 0x004699a0
// CResourceButton::`scalar deleting destructor'

#endif
