#ifndef LEMBALL_VISOS_TARGET_TARGETDIRECTDRAWSURFACECONTEXT_H
#define LEMBALL_VISOS_TARGET_TARGETDIRECTDRAWSURFACECONTEXT_H

#include "IDirectDrawSurface.h"
#include "TargetDibContext.h"

// SIZE 0x7c
// VTABLE: LEMBALL 0x00498828
class TargetDirectDrawSurfaceContext : public TargetDibContext {
public:
	TargetDirectDrawSurfaceContext(IDirectDrawSurface* p_surface)
	{
		m_surface = p_surface;
		m_surfaceDescription.dwSize = sizeof(DDSURFACEDESC);
	}
	virtual ~TargetDirectDrawSurfaceContext() {}
	virtual bool Lock();
	virtual bool Unlock();
	virtual unsigned char* GetBits();
	virtual int GetStride();
	DDSURFACEDESC* RefreshDescription();

private:
	IDirectDrawSurface* m_surface;      // 0x0c
	DDSURFACEDESC m_surfaceDescription; // 0x10
};

// SYNTHETIC: LEMBALL 0x004583a0
// TargetDirectDrawSurfaceContext::`scalar deleting destructor'

#endif
