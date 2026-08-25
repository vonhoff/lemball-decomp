#ifndef LEMBALL_VISOS_GRAPHICS_GDIDEVICE_H
#define LEMBALL_VISOS_GRAPHICS_GDIDEVICE_H

#include "../../Common.h"

// SIZE 0x1c
struct GdiSurfaceSlot {
	Surface* m_surface;  // 0x00
	Surface* m_parent;   // 0x04
	void* m_timer;       // 0x08
	int m_flushed;       // 0x0c
	int m_isPrimary;     // 0x10
	int m_available;     // 0x14
	int m_reserved18;    // 0x18
};

// SIZE 0x14
// VTABLE: LEMBALL 0x00499d78
class GdiDevice {
public:
	GdiDevice(int p_arg0);
	int FindFreeSurface();
	int FindSurface(Surface* p_surface);
	virtual Surface* AllocateSurface(const VsRect& p_rect, Surface* p_parentSurface); // vtable+0x00
	virtual void FreeSurface(Surface* p_surface);                                     // vtable+0x04
	virtual void Sync();                                                              // vtable+0x08
	virtual void Flush(Surface* p_surface);                                           // vtable+0x0c
	~GdiDevice();

private:
	GdiSurfaceSlot* m_surfaceSlots; // 0x04
	unsigned int m_reserved08; // 0x08
	int m_primarySurfaceCount; // 0x0c
	int m_surfaceCapacity;     // 0x10
};

extern GdiDevice* g_pGdiDevice;
extern void* g_pGdiHelperTarget;
#endif
