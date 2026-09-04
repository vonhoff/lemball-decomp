#ifndef LEMBALL_VISOS_GRAPHICS_GWND_H
#define LEMBALL_VISOS_GRAPHICS_GWND_H

#include "../../Common.h"
#include "PvGWnd.h" // complete type

// SIZE 0x90
// VTABLE: LEMBALL 0x00499318
class GWnd : public PvGWnd {
public:
	GWnd();
	virtual void Create(const VsRect& p_rect, PvWnd* p_parent, char* p_title, unsigned long p_paletteId); // vtable+0xa4
	virtual void OnPaint(const VsRect& p_rect);                                                           // vtable+0xa8
	virtual void AttachPalette(unsigned long p_paletteId);                                                // vtable+0xac
	virtual void Render();                                                                                // vtable+0xb0
	virtual void Flush();                                                                                 // vtable+0xb4
	virtual int ProcessOtherMessages(unsigned int p_message,
									 unsigned int p_wParam,
									 unsigned int p_lParam);                   // vtable+0xa0
	virtual void Create(const VsRect& p_rect, PvWnd* p_parent, char* p_title); // vtable+0x04
	virtual void Move(const VsPoint& p_point);                                 // vtable+0x38
	virtual void OnCreate();                                                   // vtable+0x18
	virtual void BaseOnDestroy();                                              // vtable+0x1c
	virtual void _OnMove();                                                    // vtable+0x28
	virtual void _OnSize();                                                    // vtable+0x20
	virtual void Refresh(VsRect* p_rect);                                      // vtable+0x70
	virtual void Dummy3c();                                                    // vtable+0x3c
	virtual void OnDestroy();                                                  // vtable+0x40
	virtual void OnSize();                                                     // vtable+0x44
	~GWnd();
};

#endif
