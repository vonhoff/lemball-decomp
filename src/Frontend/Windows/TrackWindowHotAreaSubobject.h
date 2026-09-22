#ifndef LEMBALL_FRONTEND_WINDOWS_TRACKWINDOWHOTAREASUBOBJECT_H
#define LEMBALL_FRONTEND_WINDOWS_TRACKWINDOWHOTAREASUBOBJECT_H

#include "../../Visos/Foundation/CVsPoint.h" // complete type
#include "../../Visos/Foundation/CVsRect.h"  // complete type
#include "../../Visos/Graphics/CClipRect.h"  // complete type
#include "../../Visos/Graphics/CLine.h"      // complete type

class CPvGWnd;
// SIZE 0xa4
class TrackWindowHotAreaSubobject {
private:
	CVsRect m_trackRect;      // 0x38
	CLine m_line;             // 0x40
	CClipRect[4] m_clipRects; // 0x50
	int m_value;              // 0x90
	CVsPoint m_trackSize;     // 0x94
	CPvGWnd* m_parent;        // 0x9c
	int m_contextId;          // 0xa0
};

#endif
