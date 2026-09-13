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
};

#endif
