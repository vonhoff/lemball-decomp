#ifndef LEMBALL_FRONTEND_WINDOWS_CTRACKWINDOW_H
#define LEMBALL_FRONTEND_WINDOWS_CTRACKWINDOW_H

#include "Engine/Graphics/Primitives/CLine.h"
#include "Engine/Graphics/Primitives/CSolidRect.h"
#include "Engine/Input/CHotAreaHandler.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Queues/tagMESSAGE.h"
#include "Platform/Windows/Windowing/CGWnd.h"

class CPVGWnd;

// SIZE 0x134
// VTABLE: LEMBALL 0x00497f48 CGWnd
// VTABLE: LEMBALL 0x00497f20 CHotAreaHandler
class CTrackWindow : public CGWnd, public CHotAreaHandler {
public:
	CTrackWindow(const CVSRect& p_rect, int p_value, CPVGWnd* p_parent);
	virtual unsigned int GetStyle();                                               // vtable+0x64
	virtual void OnButtonDown(const CVSPoint& p_point, eMouseButtonIndex p_flags); // vtable+0x04
	virtual void Create(const CVSRect& p_rect, CPVWnd* p_parent, char* p_name);    // vtable+0x04
	virtual void Move(const CVSPoint& p_position);                                 // vtable+0x38
	virtual void OnCreate();                                                       // vtable+0x3c
	virtual void OnDriverChange();                                                 // vtable+0x5c
	virtual void OnInside(const CVSPoint& p_point);                                // vtable+0x18
	virtual void OnPaint(const CVSRect& p_rect);                                   // vtable+0xa8
	virtual ~CTrackWindow();                                                       // vtable+0x00
	void SetButtonValue(int p_value);

private:
	friend class CGunController;
	friend class CGunButtons;
	CVSRect m_trackRect;        // 0xc8
	CSolidRect m_line;          // 0xd0
	CLine m_edges[4];           // 0xe0
	int m_value;                // 0x120
	int m_trackWidth;           // 0x124
	unsigned int m_reserved128; // 0x128
	CPVGWnd* m_parent;          // 0x12c
	int m_contextId;            // 0x130
};

// SYNTHETIC: LEMBALL 0x0044eff0
// CTrackWindow::`scalar deleting destructor'

#endif
