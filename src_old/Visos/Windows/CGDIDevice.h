#ifndef LEMBALL_VISOS_WINDOWS_CGDIDEVICE_H
#define LEMBALL_VISOS_WINDOWS_CGDIDEVICE_H

struct VsGdiResourceGeometryHelperSlot;

// Windows ABI layout recovered from /LEMBALL.EXE. Method names come from
// CODE_01 `Visos (Mac Specific)`; Macintosh layout is not transferred.
class CGDIDevice {
public:
	virtual void* AllocateSurface(short* pRect, void* pSurface);
	virtual void FreeSurface(void* pSurface);
	virtual void Sync(void);
	virtual void Flush(void* pSurface);

	int FindSurface(void* pSurface);

	VsGdiResourceGeometryHelperSlot* m_pSlots;
	int m_nReserved08;
	int m_nReserved0C;
	int m_cSlots;

	CGDIDevice(int cSlots);
	~CGDIDevice(void);
};

#endif
