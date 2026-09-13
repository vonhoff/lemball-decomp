#ifndef LEMBALL_VISOS_TARGET_IDIRECTDRAW_H
#define LEMBALL_VISOS_TARGET_IDIRECTDRAW_H

#include "IDirectDrawPalette.h"
#include "IDirectDrawSurface.h"

// DirectDraw 1 COM ABI, as declared in the Windows SDK ddraw.h.
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
