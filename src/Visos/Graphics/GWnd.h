#ifndef LEMBALL_VISOS_GRAPHICS_GWND_H
#define LEMBALL_VISOS_GRAPHICS_GWND_H

#include "../../Common.h"
#include "Wnd.h" // complete type

// SIZE 0x90
// VTABLE: LEMBALL 0x00499260
class GWnd : public Wnd {
public:
	virtual int ProcessOtherMessages(EventRecords& p_event);                                              // vtable+0xa0
	virtual void AttachPalette(unsigned long p_paletteId);                                                // vtable+0xac
	virtual void Create(const VsRect& p_rect, PvWnd* p_parent, char* p_title);                            // vtable+0x04
	virtual void Create(const VsRect& p_rect, PvWnd* p_parent, char* p_title, unsigned long p_paletteId); // vtable+0xa4
	virtual void Flush();                                                                                 // vtable+0xb4
	virtual void Move(const VsPoint& p_point);                                                            // vtable+0x38
	virtual void OnCreate();                                                                              // vtable+0x18
	virtual void OnDestroy();                                                                             // vtable+0x1c
	virtual void OnMove();                                                                                // vtable+0x28
	virtual void OnPaint(const VsRect& p_rect);                                                           // vtable+0xa8
	virtual void OnSize();                                                                                // vtable+0x20
	virtual void Refresh(VsRect* p_rect);                                                                 // vtable+0x70
	virtual void Render();                                                                                // vtable+0xb0
	~GWnd();

private:
	Gdi* m_gdi;                       // 0x4c
	unsigned int m_gdiFlags;          // 0x50
	unsigned int m_paletteResourceId; // 0x54
	undefined m_renderState0[0x10];   // 0x58
	undefined m_renderState1[0x10];   // 0x68
	undefined m_renderState2[0x10];   // 0x78
	unsigned short m_refreshWidth;    // 0x88
	unsigned short m_refreshHeight;   // 0x8a
	unsigned short m_refreshX;        // 0x8c
	unsigned short m_refreshY;        // 0x8e
};

#endif
