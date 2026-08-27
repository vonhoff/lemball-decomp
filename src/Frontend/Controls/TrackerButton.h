#ifndef LEMBALL_FRONTEND_CONTROLS_TRACKERBUTTON_H
#define LEMBALL_FRONTEND_CONTROLS_TRACKERBUTTON_H

#include "../../Common.h"
#include "../../Visos/Graphics/GraphicButton.h" // complete type

// SIZE 0x138
// VTABLE: LEMBALL 0x00498050 primary
// VTABLE: LEMBALL 0x00498028 adjusted hot-area subobject at +0x90
class TrackerButton : public GraphicButton {
public:
	TrackerButton(const VsPoint& p_arg0, PvGWnd* p_arg1, unsigned long p_arg2, VsRect& p_arg3, int p_arg4);
	virtual void Move(const VsPoint& p_point); // vtable+0x38
	virtual ~TrackerButton();                  // vtable+0x00

private:
	short m_trackOffsetX;       // 0x130
	short m_trackOffsetY;       // 0x132
	TrackWindow* m_trackWindow; // 0x134
};

// SYNTHETIC: LEMBALL 0x0044f030
// TrackerButton::`scalar deleting destructor'


#endif

