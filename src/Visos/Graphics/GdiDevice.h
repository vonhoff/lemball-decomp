#ifndef LEMBALL_VISOS_GRAPHICS_GDIDEVICE_H
#define LEMBALL_VISOS_GRAPHICS_GDIDEVICE_H

#include "../../Common.h"

// SIZE 0x14
// VTABLE: LEMBALL 0x00499d78
class GdiDevice {
public:
	GdiDevice(int p_arg0);
	int FindFreeSurface();
	int FindSurface(Surface* p_surface);
	virtual Surface* AllocateSurface(const VsRect& p_rect, Surface* p_parentSurface); // vtable+0x00
	virtual void Flush(Surface* p_surface);                                           // vtable+0x0c
	virtual void FreeSurface(Surface* p_surface);                                     // vtable+0x04
	virtual void Sync();                                                              // vtable+0x08
	~GdiDevice();

private:
	undefined* m_surfaceSlots; // 0x04
	unsigned int m_reserved08; // 0x08
	int m_primarySurfaceCount; // 0x0c
	int m_surfaceCapacity;     // 0x10
};

#endif
