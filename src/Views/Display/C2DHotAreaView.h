#ifndef LEMBALL_VIEWS_DISPLAY_C2DHOTAREAVIEW_H
#define LEMBALL_VIEWS_DISPLAY_C2DHOTAREAVIEW_H

#include "../../Visos/Foundation/CVsPoint.h"

class CMain2DDisplay;
class CMap;
// SIZE 0x2414
class C2DHotAreaView {
private:
	unsigned int m_mouseDown;  // 0x178
	CMap* m_map;               // 0x900
	int m_viewOriginX;         // 0x904
	int m_viewOriginY;         // 0x908
	CVsPoint m_cursorPoint;    // 0x928
	CMain2DDisplay* m_display; // 0x964
	unsigned int m_paused;     // 0xa68
};

#endif
