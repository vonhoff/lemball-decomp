#ifndef LEMBALL_FRONTEND_CONTROLS_CTRACKERBUTTON_H
#define LEMBALL_FRONTEND_CONTROLS_CTRACKERBUTTON_H

#include "../../Visos/Graphics/CGraphicButton.h" // complete type

class CPvGWnd;
class CTrackWindow;
class CVsRect;
struct CVsPoint;
// SIZE 0x138
// VTABLE: LEMBALL 0x00498050 CGWnd
// VTABLE: LEMBALL 0x00498028 CHotAreaHandler
class CTrackerButton : public CGraphicButton {
public:
	CTrackerButton(const CVsPoint& p_arg0, CPvGWnd* p_arg1, unsigned long p_arg2, CVsRect& p_arg3, int p_arg4);
	virtual void Move(const CVsPoint& p_point); // vtable+0x38
	virtual ~CTrackerButton();                  // vtable+0x00

private:
	friend class CGunController;
	friend class CGunButtons;
	short m_trackOffsetX;        // 0x130
	short m_trackOffsetY;        // 0x132
	CTrackWindow* m_trackWindow; // 0x134
};

// SYNTHETIC: LEMBALL 0x0044f030
// CTrackerButton::`scalar deleting destructor'

#endif
