#ifndef LEMBALL_PLATFORM_DIRECTX_IDIRECTDRAWSURFACE_H
#define LEMBALL_PLATFORM_DIRECTX_IDIRECTDRAWSURFACE_H

#include "DDBLTFX.h"
#include "DDSURFACEDESC.h"
#include "IDirectDrawPalette.h"

// DirectDraw 1 COM ABI from the Windows SDK ddraw.h.
// SDK-only pointer types not accessed here remain opaque.
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

#endif
