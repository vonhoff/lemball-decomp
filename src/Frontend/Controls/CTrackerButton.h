#ifndef LEMBALL_FRONTEND_CONTROLS_CTRACKERBUTTON_H
#define LEMBALL_FRONTEND_CONTROLS_CTRACKERBUTTON_H

#include "Engine/Math/CVSPoint.h"
#include "Platform/Windows/Windowing/CGraphicButton.h"

class CPVGWnd;
class CTrackWindow;
class CVSRect;
// SIZE 0x138
// VTABLE: LEMBALL 0x00498050 CGWnd
// VTABLE: LEMBALL 0x00498028 CHotAreaHandler
class CTrackerButton : public CGraphicButton {
public:
	CTrackerButton(const CVSPoint& p_position,
				   CPVGWnd* p_parent,
				   unsigned long p_animId,
				   CVSRect& p_trackRect,
				   int p_value);
	virtual void Move(const CVSPoint& p_point); // vtable+0x38
	virtual ~CTrackerButton();                  // vtable+0x00

private:
	friend class CGunController;
	friend class CGunButtons;
	CVSPoint m_trackOffset;      // 0x130
	CTrackWindow* m_trackWindow; // 0x134
};

// SYNTHETIC: LEMBALL 0x0044f030
// CTrackerButton::`scalar deleting destructor'

#endif
