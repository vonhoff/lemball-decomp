#ifndef LEMBALL_VISOS_GRAPHICS_PVSURFACE_H
#define LEMBALL_VISOS_GRAPHICS_PVSURFACE_H

#include "../../Common.h"
#include "../Foundation/VsRect.h" // complete type

// SIZE 0x44
// VTABLE: LEMBALL 0x00499508
class PvSurface {
public:
	PvSurface();
	virtual void Blit(Zrle* p_primitive, ResZrle* p_zrle) = 0;            // vtable+0x24
	virtual void Blit(ScreenScroll* p_scroll) = 0;                        // vtable+0x20
	virtual void Blit(ClipRect* p_arg0) = 0;                              // vtable+0x1c
	virtual void Blit(BigBitmap* p_arg0, ResBitmap* p_arg1) = 0;          // vtable+0x18
	virtual void Blit(Bitmap* p_primitive, ResBitmap* p_bitmap) = 0;      // vtable+0x14
	virtual void Blit(FilledCircle* p_circle) = 0;                        // vtable+0x10
	virtual void Blit(Circle* p_circle) = 0;                              // vtable+0x0c
	virtual void Blit(SolidRect* p_rect) = 0;                             // vtable+0x08
	virtual void Blit(Line* p_line) = 0;                                  // vtable+0x04
	virtual void Blit(Point* p_point) = 0;                                // vtable+0x00
	virtual void Resize(const VsSize& p_size);                            // vtable+0x28
	virtual void SetWindowPtr(void* p_platformPort) = 0;                  // vtable+0x2c
	virtual void Flush() = 0;                                             // vtable+0x30
	virtual void SetDefaultCtable() = 0;                                  // vtable+0x34
	virtual void* GetCurrDb() = 0;                                        // vtable+0x38
	virtual bool HasBackBuff();                                           // vtable+0x3c
	virtual bool HasZBuff();                                              // vtable+0x40
	virtual void SetWorldWidth(int p_width);                              // vtable+0x44
	virtual int GetWorldWidth();                                          // vtable+0x48
	void SetDontUpdateRect(const VsRect& p_rect);
	~PvSurface();

private:
	VsRect m_dontUpdateRect;         // 0x04
	undefined m_geometryState[0x30]; // 0x0c
	unsigned int m_updateEnabled;    // 0x3c
	void* m_reserved40;              // 0x40
};

#endif
