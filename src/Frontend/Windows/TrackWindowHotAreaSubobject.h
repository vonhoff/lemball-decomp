#ifndef LEMBALL_FRONTEND_WINDOWS_TRACKWINDOWHOTAREASUBOBJECT_H
#define LEMBALL_FRONTEND_WINDOWS_TRACKWINDOWHOTAREASUBOBJECT_H

#include "../../Common.h"
#include "../../Visos/Foundation/VsPoint.h" // complete type
#include "../../Visos/Foundation/VsRect.h"  // complete type
#include "../../Visos/Graphics/ClipRect.h"  // complete type
#include "../../Visos/Graphics/Line.h"      // complete type

// SIZE 0xa4
class TrackWindowHotAreaSubobject {
public:
	// No functions mapped yet.

private:
	VsRect m_trackRect;      // 0x38
	Line m_line;             // 0x40
	ClipRect[4] m_clipRects; // 0x50
	int m_value;             // 0x90
	VsPoint m_trackSize;     // 0x94
	PvGWnd* m_parent;        // 0x9c
	int m_contextId;         // 0xa0
};

#endif
