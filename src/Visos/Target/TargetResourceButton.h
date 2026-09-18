#ifndef LEMBALL_VISOS_TARGET_TARGETRESOURCEBUTTON_H
#define LEMBALL_VISOS_TARGET_TARGETRESOURCEBUTTON_H

#include "TargetFramedButton.h" // complete type

class GWnd;
class HotAreaHandler;
class VsRect;
// MINIMUM SIZE 0x11c
// VTABLE: LEMBALL 0x00499928 GWnd
// VTABLE: LEMBALL 0x00499908 HotAreaHandler
class TargetResourceButton : public TargetFramedButton {
public:
	virtual ~TargetResourceButton();
	virtual void OnPaint(const VsRect& p_rect);
	virtual void OnDestroy();

private:
	void* m_resourceInterface; // 0x118
};

// SYNTHETIC: LEMBALL 0x004699a0
// TargetResourceButton::`scalar deleting destructor'

#endif
