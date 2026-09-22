#ifndef LEMBALL_VISOS_TARGET_GRAPHICS_CDIRECTDRAWSURFACE_H
#define LEMBALL_VISOS_TARGET_GRAPHICS_CDIRECTDRAWSURFACE_H

#include "CDibContext.h"
#include "Platform/DirectX/DDSURFACEDESC.h"

struct IDirectDrawSurface;

// SIZE 0x7c
// VTABLE: LEMBALL 0x00498828
class CDirectDrawSurface : public CDibContext {
public:
	CDirectDrawSurface(IDirectDrawSurface* p_surface)
	{
		m_surface = p_surface;
		m_surfaceDescription.dwSize = sizeof(DDSURFACEDESC);
	}
	virtual ~CDirectDrawSurface() {}
	virtual bool Lock();
	virtual bool Unlock();
	virtual unsigned char* GetBits();
	virtual int GetStride();
	DDSURFACEDESC* RefreshDescription();
	friend class CDirectDrawDriver;

private:
	IDirectDrawSurface* m_surface;      // 0x0c
	DDSURFACEDESC m_surfaceDescription; // 0x10
};

// SYNTHETIC: LEMBALL 0x004583a0
// CDirectDrawSurface::`scalar deleting destructor'

#endif
