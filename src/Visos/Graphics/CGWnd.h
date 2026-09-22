#ifndef LEMBALL_VISOS_GRAPHICS_CGWND_H
#define LEMBALL_VISOS_GRAPHICS_CGWND_H

#include "CPVGWnd.h" // complete type

// SIZE 0x90
// VTABLE: LEMBALL 0x00499318
class CGWnd : public CPVGWnd {
public:
	CGWnd();
	virtual void Create(const CVsRect& p_rect,
						CPVWnd* p_parent,
						char* p_title,
						unsigned long p_paletteId);        // vtable+0xa4
	virtual void OnPaint(const CVsRect& p_rect);           // vtable+0xa8
	virtual void AttachPalette(unsigned long p_paletteId); // vtable+0xac
	virtual void Render();                                 // vtable+0xb0
	virtual void Flush();                                  // vtable+0xb4
	virtual int ProcessOtherMessages(unsigned int p_message,
									 unsigned int p_wParam,
									 unsigned int p_lParam);                     // vtable+0xa0
	virtual void Create(const CVsRect& p_rect, CPVWnd* p_parent, char* p_title); // vtable+0x04
	virtual void Move(const CVsPoint& p_point);                                  // vtable+0x38
	virtual void _OnCreate();                                                    // vtable+0x18
	virtual void _OnDestroy();                                                   // vtable+0x1c
	virtual void _OnMove();                                                      // vtable+0x28
	virtual void _OnSize();                                                      // vtable+0x20
	virtual void Refresh(CVsRect* p_rect);                                       // vtable+0x70
	virtual void OnCreate();                                                     // vtable+0x3c
	virtual void OnDestroy();                                                    // vtable+0x40
	virtual void OnSize();                                                       // vtable+0x44
	~CGWnd();
};

#endif
