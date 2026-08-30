#ifndef LEMBALL_FRONTEND_WINDOWS_TRACKWINDOW_H
#define LEMBALL_FRONTEND_WINDOWS_TRACKWINDOW_H

#include "../../Common.h"
#include "../../Visos/Foundation/VsPoint.h"      // complete type
#include "../../Visos/Foundation/VsRect.h"       // complete type
#include "../../Visos/Graphics/ClipRect.h"       // complete type
#include "../../Visos/Graphics/HotAreaHandler.h" // complete type
#include "../../Visos/Graphics/Line.h"           // complete type
#include "../../Visos/Graphics/PvGWnd.h"         // complete type

// SIZE 0x134
// VTABLE: LEMBALL 0x00497f48 primary PvGWnd base at +0x00
// VTABLE: LEMBALL 0x00497f20 adjusted HotAreaHandler base at +0x90
class TrackWindow : public PvGWnd, public HotAreaHandler {
public:
	TrackWindow(const VsRect& p_arg0, int p_arg1, PvGWnd* p_arg2);
	virtual unsigned int GetStyle();                                          // vtable+0x64
	virtual void OnButtonDown(const VsPoint& p_point, int p_flags);           // vtable+0x04
	virtual void Create(const VsRect& p_rect, PvWnd* p_parent, char* p_name); // vtable+0x04
	virtual void Move(const VsPoint& p_position);                             // vtable+0x38
	virtual void Dummy3c();                                                   // vtable+0x3c
	virtual void OnDriverChange();                                            // vtable+0x5c
	virtual void OnInside(const VsPoint& p_point);                            // vtable+0x18
	virtual void OnPaint(const VsRect& p_rect);                               // vtable+0xa8
	virtual ~TrackWindow();                                                   // vtable+0x00
	void SetButtonValue(int p_value);

private:
	VsRect m_trackRect;         // 0xc8
	Line m_line;                // 0xd0
	ClipRect m_clipRects[4];    // 0xe0
	int m_value;                // 0x120
	VsPoint m_trackSize;        // 0x124
	unsigned int m_reserved128; // 0x128
	PvGWnd* m_parent;           // 0x12c
	int m_contextId;            // 0x130
};

// SYNTHETIC: LEMBALL 0x0044eff0
// TrackWindow::`scalar deleting destructor'

#endif
