#ifndef LEMBALL_VISOS_GRAPHICS_PVGWND_H
#define LEMBALL_VISOS_GRAPHICS_PVGWND_H

#include "../../Common.h"
#include "GWnd.h" // complete type

// SIZE 0x90
// VTABLE: LEMBALL 0x00499318
class PvGWnd : public GWnd {
public:
	PvGWnd();
	virtual unsigned int GetStyle();                      // vtable+0x64
	virtual void OnVisibilityChange();                    // vtable+0x80
	virtual void _OnZoom(int p_oldZoom);                  // vtable+0x2c
	virtual void SetDontUpdateRect(const VsRect& p_rect); // vtable+0x84
	virtual void SetInnerWindow(const VsRect& p_rect);    // vtable+0x08
	virtual void Dummy3c();                               // vtable+0x3c
	virtual void OnDestroy();                             // vtable+0x40
	virtual void OnSize();                                // vtable+0x44
	void Clear(int p_color);
};

#endif
