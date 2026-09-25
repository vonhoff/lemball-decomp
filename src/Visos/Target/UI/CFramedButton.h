#ifndef LEMBALL_VISOS_TARGET_UI_CFRAMEDBUTTON_H
#define LEMBALL_VISOS_TARGET_UI_CFRAMEDBUTTON_H

#include "../../Graphics/CDepressedButton.h"

class CClipRect;
class CLine;
class CPVGWnd;
class CVsRect;
// SIZE 0x118
// VTABLE: LEMBALL 0x00499838 CGWnd
// VTABLE: LEMBALL 0x00499818 CHotAreaHandler
class CFramedButton : public CDepressedButton {
public:
	CFramedButton(const CVsRect& p_rect, CPVGWnd* p_parent, unsigned int p_frameColor);
	CFramedButton(CPVGWnd* p_parent, unsigned int p_frameColor);
	void InitializeFramePrimitives();
	virtual ~CFramedButton();
	virtual void DrawButton();
	virtual void OnPaint(const CVsRect& p_rect);

private:
	CLine* m_frameLine;        // 0x10c
	CClipRect* m_frameRects;   // 0x110
	unsigned int m_frameColor; // 0x114
};

// SYNTHETIC: LEMBALL 0x00469900
// CFramedButton::`scalar deleting destructor'

#endif
