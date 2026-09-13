#ifndef LEMBALL_VISOS_TARGET_IDIRECTDRAWPALETTE_H
#define LEMBALL_VISOS_TARGET_IDIRECTDRAWPALETTE_H

struct IDirectDraw;

// DirectDraw 1 COM ABI from the Windows SDK ddraw.h.
struct IDirectDrawPalette {
	virtual long __stdcall QueryInterface(const void*, void**) = 0;
	virtual unsigned long __stdcall AddRef() = 0;
	virtual unsigned long __stdcall Release() = 0;
	virtual long __stdcall GetCaps(unsigned long*) = 0;
	virtual long __stdcall GetEntries(unsigned long, unsigned long, unsigned long, void*) = 0;
	virtual long __stdcall Initialize(IDirectDraw*, unsigned long, void*) = 0;
	virtual long __stdcall SetEntries(unsigned long, unsigned long, unsigned long, void*) = 0;
};

#endif
