#ifndef LEMBALL_VISOS_TARGET_TARGETFRAMEDBUTTON_H
#define LEMBALL_VISOS_TARGET_TARGETFRAMEDBUTTON_H

#include "../Graphics/CDepressedButton.h" // complete type

class CClipRect;
class CLine;
class CPvGWnd;
class CVsRect;
// SIZE 0x118
// VTABLE: LEMBALL 0x00499838 CGWnd
// VTABLE: LEMBALL 0x00499818 CHotAreaHandler
class TargetFramedButton : public CDepressedButton {
public:
	TargetFramedButton(const CVsRect& p_rect, CPvGWnd* p_parent, unsigned int p_frameColor);
	TargetFramedButton(CPvGWnd* p_parent, unsigned int p_frameColor);
	void InitializeFramePrimitives();
	virtual ~TargetFramedButton();
	virtual void DrawButton();
	virtual void OnPaint(const CVsRect& p_rect);

private:
	CLine* m_frameLine;        // 0x10c
	CClipRect* m_frameRects;   // 0x110
	unsigned int m_frameColor; // 0x114
};

// SYNTHETIC: LEMBALL 0x00469900
// TargetFramedButton::`scalar deleting destructor'

#endif
