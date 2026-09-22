#ifndef LEMBALL_VISOS_GRAPHICS_CGDIDEVICE_H
#define LEMBALL_VISOS_GRAPHICS_CGDIDEVICE_H

class CSurface;
class CTimeStat;
class CVsRect;

// SIZE 0x1c
struct GdiSurfaceSlot {
	CSurface* m_surface; // 0x00
	CSurface* m_parent;  // 0x04
	CTimeStat* m_timer;  // 0x08
	int m_flushed;       // 0x0c
	int m_isPrimary;     // 0x10
	int m_available;     // 0x14
	int m_reserved18;    // 0x18
};

// SIZE 0x14
// VTABLE: LEMBALL 0x00499d78
class CGDIDevice {
public:
	CGDIDevice(int p_surfaceCapacity);
	int FindFreeSurface();
	int FindSurface(CSurface* p_surface);
	virtual CSurface* AllocateSurface(const CVsRect& p_rect, CSurface* p_parentSurface); // vtable+0x00
	virtual void FreeSurface(CSurface* p_surface);                                       // vtable+0x04
	virtual void Sync();                                                                 // vtable+0x08
	virtual void Flush(CSurface* p_surface);                                             // vtable+0x0c
	~CGDIDevice();

private:
	GdiSurfaceSlot* m_surfaceSlots; // 0x04
	unsigned int m_reserved08;      // 0x08
	int m_primarySurfaceCount;      // 0x0c
	int m_surfaceCapacity;          // 0x10
};

extern CGDIDevice* g_pGdiDevice;
extern void* g_pGdiHelperTarget;
#endif
