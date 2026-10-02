#ifndef LEMBALL_VISOS_TARGET_UI_CFRAMEDBUTTON_H
#define LEMBALL_VISOS_TARGET_UI_CFRAMEDBUTTON_H

#include "../../Graphics/CDepressedButton.h"

class CLine;
class CSolidRect;
class CPVGWnd;
class CVSRect;
// SIZE 0x118
// VTABLE: LEMBALL 0x00499838 CGWnd
// VTABLE: LEMBALL 0x00499818 CHotAreaHandler
class CFramedButton : public CDepressedButton {
public:
	CFramedButton(const CVSRect& p_rect, CPVGWnd* p_parent, unsigned int p_frameColour);
	CFramedButton(CPVGWnd* p_parent, unsigned int p_frameColour);
	void InitializeFramePrimitives();
	virtual ~CFramedButton();
	virtual void DrawButton();
	virtual void OnPaint(const CVSRect& p_rect);

private:
	CSolidRect* m_frameLine;    // 0x10c
	CLine* m_frameEdges;        // 0x110
	unsigned int m_frameColour; // 0x114
};

// SYNTHETIC: LEMBALL 0x00469900
// CFramedButton::`scalar deleting destructor'

#endif
