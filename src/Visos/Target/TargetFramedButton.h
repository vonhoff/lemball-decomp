#ifndef LEMBALL_VISOS_TARGET_TARGETFRAMEDBUTTON_H
#define LEMBALL_VISOS_TARGET_TARGETFRAMEDBUTTON_H

#include "../../Common.h"
#include "../Graphics/DepressedButton.h" // complete type

// SIZE 0x118
// VTABLE: LEMBALL 0x00499838 GWnd
// VTABLE: LEMBALL 0x00499818 HotAreaHandler
class TargetFramedButton : public DepressedButton {
public:
	TargetFramedButton(const VsRect& p_rect, PvGWnd* p_parent, unsigned int p_frameColor);
	TargetFramedButton(PvGWnd* p_parent, unsigned int p_frameColor);
	void InitializeFramePrimitives();
	virtual ~TargetFramedButton();
	virtual void DrawButton();
	virtual void OnPaint(const VsRect& p_rect);

private:
	Line* m_frameLine;         // 0x10c
	ClipRect* m_frameRects;    // 0x110
	unsigned int m_frameColor; // 0x114
};

// SYNTHETIC: LEMBALL 0x00469900
// TargetFramedButton::`scalar deleting destructor'

#endif
