#ifndef LEMBALL_FRONTEND_WINDOWS_CTRACKHOTAREAVIEW_H
#define LEMBALL_FRONTEND_WINDOWS_CTRACKHOTAREAVIEW_H

#include "Visos/Math/CVSPoint.h"
#include "Visos/Math/CVSRect.h"
#include "Visos/Graphics/Primitives/CLine.h"
#include "Visos/Graphics/Primitives/CSolidRect.h"

class CPVGWnd;
// SIZE 0xa4
class CTrackHotAreaView {
private:
	CVSRect m_trackRect;  // 0x38
	CSolidRect m_line;    // 0x40
	CLine[4] m_clipRects; // 0x50
	int m_value;          // 0x90
	CVSPoint m_trackSize; // 0x94
	CPVGWnd* m_parent;    // 0x9c
	int m_contextId;      // 0xa0
};

#endif
