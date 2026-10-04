#ifndef LEMBALL_PLATFORM_DIRECTX_DIRECTDRAW_H
#define LEMBALL_PLATFORM_DIRECTX_DIRECTDRAW_H

struct IDirectDraw;

struct IDirectDrawPalette {
	virtual long __stdcall QueryInterface(const void*, void**) = 0;
	virtual unsigned long __stdcall AddRef() = 0;
	virtual unsigned long __stdcall Release() = 0;
	virtual long __stdcall GetCaps(unsigned long*) = 0;
	virtual long __stdcall GetEntries(unsigned long, unsigned long, unsigned long, void*) = 0;
	virtual long __stdcall Initialize(IDirectDraw*, unsigned long, void*) = 0;
	virtual long __stdcall SetEntries(unsigned long, unsigned long, unsigned long, void*) = 0;
};

struct DDBLTFX {
	unsigned long dwSize;
	char m_unused04[0x4c];
	unsigned long dwFillColor;
	char m_unused54[0x10];
};

#define DDSCAPS_PRIMARYSURFACE 0x00000200

struct DDSURFACEDESC {
	unsigned long dwSize;
	unsigned long dwFlags;
	unsigned long dwHeight;
	unsigned long dwWidth;
	long lPitch;
	char m_unused14[0x10];
	void* lpSurface;
	char m_unused28[0x40];
	unsigned long ddsCaps;
};

struct IDirectDrawSurface {
	virtual long __stdcall QueryInterface(const void*, void**) = 0;
	virtual unsigned long __stdcall AddRef() = 0;
	virtual unsigned long __stdcall Release() = 0;
	virtual long __stdcall AddAttachedSurface(IDirectDrawSurface*) = 0;
	virtual long __stdcall AddOverlayDirtyRect(void*) = 0;
	virtual long __stdcall Blt(void*, IDirectDrawSurface*, void*, unsigned long, DDBLTFX*) = 0;
	virtual long __stdcall BltBatch(void*, unsigned long, unsigned long) = 0;
	virtual long __stdcall BltFast(unsigned long, unsigned long, IDirectDrawSurface*, void*, unsigned long) = 0;
	virtual long __stdcall DeleteAttachedSurface(unsigned long, IDirectDrawSurface*) = 0;
	virtual long __stdcall EnumAttachedSurfaces(void*,
												long(__stdcall*)(IDirectDrawSurface*, DDSURFACEDESC*, void*)) = 0;
	virtual long __stdcall EnumOverlayZOrders(unsigned long,
											  void*,
											  long(__stdcall*)(IDirectDrawSurface*, DDSURFACEDESC*, void*)) = 0;
	virtual long __stdcall Flip(IDirectDrawSurface*, unsigned long) = 0;
	virtual long __stdcall GetAttachedSurface(void*, IDirectDrawSurface**) = 0;
	virtual long __stdcall GetBltStatus(unsigned long) = 0;
	virtual long __stdcall GetCaps(void*) = 0;
	virtual long __stdcall GetClipper(void**) = 0;
	virtual long __stdcall GetColorKey(unsigned long, void*) = 0;
	virtual long __stdcall GetDC(void**) = 0;
	virtual long __stdcall GetFlipStatus(unsigned long) = 0;
	virtual long __stdcall GetOverlayPosition(long*, long*) = 0;
	virtual long __stdcall GetPalette(IDirectDrawPalette**) = 0;
	virtual long __stdcall GetPixelFormat(void*) = 0;
	virtual long __stdcall GetSurfaceDesc(DDSURFACEDESC*) = 0;
	virtual long __stdcall Initialize(IDirectDraw*, DDSURFACEDESC*) = 0;
	virtual long __stdcall IsLost() = 0;
	virtual long __stdcall Lock(void*, DDSURFACEDESC*, unsigned long, void*) = 0;
	virtual long __stdcall ReleaseDC(void*) = 0;
	virtual long __stdcall Restore() = 0;
	virtual long __stdcall SetClipper(void*) = 0;
	virtual long __stdcall SetColorKey(unsigned long, void*) = 0;
	virtual long __stdcall SetOverlayPosition(long, long) = 0;
	virtual long __stdcall SetPalette(IDirectDrawPalette*) = 0;
	virtual long __stdcall Unlock(void*) = 0;
};

#define DDSCL_FULLSCREEN 0x00000001
#define DDSCL_NORMAL 0x00000008
#define DDSCL_EXCLUSIVE 0x00000010

struct IDirectDraw {
	virtual long __stdcall QueryInterface(const void*, void**) = 0;
	virtual unsigned long __stdcall AddRef() = 0;
	virtual unsigned long __stdcall Release() = 0;
	virtual long __stdcall Compact() = 0;
	virtual long __stdcall CreateClipper(unsigned long, void**, void*) = 0;
	virtual long __stdcall CreatePalette(unsigned long, void*, IDirectDrawPalette**, void*) = 0;
	virtual long __stdcall CreateSurface(DDSURFACEDESC*, IDirectDrawSurface**, void*) = 0;
	virtual long __stdcall DuplicateSurface(IDirectDrawSurface*, IDirectDrawSurface**) = 0;
	virtual long __stdcall EnumDisplayModes(unsigned long,
											DDSURFACEDESC*,
											void*,
											long(__stdcall*)(DDSURFACEDESC*, void*)) = 0;
	virtual long __stdcall EnumSurfaces(unsigned long,
										DDSURFACEDESC*,
										void*,
										long(__stdcall*)(IDirectDrawSurface*, DDSURFACEDESC*, void*)) = 0;
	virtual long __stdcall FlipToGDISurface() = 0;
	virtual long __stdcall GetCaps(void*, void*) = 0;
	virtual long __stdcall GetDisplayMode(DDSURFACEDESC*) = 0;
	virtual long __stdcall GetFourCCCodes(unsigned long*, unsigned long*) = 0;
	virtual long __stdcall GetGDISurface(IDirectDrawSurface**) = 0;
	virtual long __stdcall GetMonitorFrequency(unsigned long*) = 0;
	virtual long __stdcall GetScanLine(unsigned long*) = 0;
	virtual long __stdcall GetVerticalBlankStatus(int*) = 0;
	virtual long __stdcall Initialize(void*) = 0;
	virtual long __stdcall RestoreDisplayMode() = 0;
	virtual long __stdcall SetCooperativeLevel(void*, unsigned long) = 0;
	virtual long __stdcall SetDisplayMode(unsigned long, unsigned long, unsigned long) = 0;
};

#endif
