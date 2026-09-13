#ifndef LEMBALL_VISOS_TARGET_IDIRECTDRAWSURFACE_H
#define LEMBALL_VISOS_TARGET_IDIRECTDRAWSURFACE_H

#include "DDSURFACEDESC.h"

// DirectDraw 1 COM ABI from the Windows SDK ddraw.h. Unused slots retain
// their position without assigning unverified application types to them.
struct IDirectDrawSurface {
	struct Vtable {
		undefined4 m_slots00[22];
		long(__stdcall* GetSurfaceDesc)(IDirectDrawSurface*, DDSURFACEDESC*);
		undefined4 m_slots5c[2];
		long(__stdcall* Lock)(IDirectDrawSurface*, void*, DDSURFACEDESC*, unsigned long, void*);
		undefined4 m_slots68[6];
		long(__stdcall* Unlock)(IDirectDrawSurface*, void*);
	};
	Vtable* lpVtbl;
};

#endif
