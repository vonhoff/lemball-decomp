#ifndef LEMBALL_FRONTEND_WINDOWS_CTRACKHOTAREAVIEW_H
#define LEMBALL_FRONTEND_WINDOWS_CTRACKHOTAREAVIEW_H

#include "../../Visos/Foundation/CVSPoint.h"
#include "../../Visos/Foundation/CVSRect.h"
#include "../../Visos/Graphics/CLine.h"
#include "../../Visos/Graphics/CSolidRect.h"

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
