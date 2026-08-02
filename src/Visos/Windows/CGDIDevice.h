#ifndef LEMBALL_VISOS_WINDOWS_CGDIDEVICE_H
#define LEMBALL_VISOS_WINDOWS_CGDIDEVICE_H

class CSurface;
class CVSRect;

// Platform analogue of CODE_01 `Visos (Mac Specific)`; no Macintosh layout is transferred.
class CGDIDevice {
public:
	CGDIDevice(int nDevice);
	~CGDIDevice(void);
	CSurface* FindFreeSurface(void);
	CSurface* AllocateSurface(const CVSRect& rect, CSurface* pSurface);
	void FreeSurface(CSurface* pSurface);
	void Sync(void);
	CSurface* FindSurface(CSurface* pSurface);
	void Flush(CSurface* pSurface);
};

#endif
