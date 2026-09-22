#include "CDirectDrawSurface.h"

#include "../Foundation/CVsOStream.h"
#include "DirectDrawError.h"
#include "Visos/Target/IDirectDrawSurface.h"

// FUNCTION: LEMBALL 0x00457310
DDSURFACEDESC* CDirectDrawSurface::RefreshDescription()
{
	long result = m_surface->GetSurfaceDesc(&m_surfaceDescription);
	if (result != 0) {
		*g_pErrorOutput << "Direct Draw Surface Get Description failed: "
						<< FormatUnknownDirectDrawError(result & 0xfff) << "\n";
		return 0;
	}
	return &m_surfaceDescription;
}

// FUNCTION: LEMBALL 0x00457360
unsigned char* CDirectDrawSurface::GetBits()
{
	if (m_bits == 0) {
		Lock();
		Unlock();
	}
	return m_bits;
}

// FUNCTION: LEMBALL 0x00457380
int CDirectDrawSurface::GetStride()
{
	if (m_width == 0) {
		Lock();
		Unlock();
	}
	return m_width;
}

// FUNCTION: LEMBALL 0x004573a0
bool CDirectDrawSurface::Lock()
{
	long result;
	while ((result = m_surface->Lock(0, &m_surfaceDescription, 0, 0)) != 0) {
		if (result == (long) 0x887601c2) {
			return 0;
		}
	}
	m_bits = (unsigned char*) m_surfaceDescription.lpSurface;
	m_width = m_surfaceDescription.lPitch;
	return 1;
}

// FUNCTION: LEMBALL 0x004573e0
bool CDirectDrawSurface::Unlock()
{
	long result;
	while ((result = m_surface->Unlock(m_bits)) != 0) {
		if (result == (long) 0x887601c2) {
			return 0;
		}
	}
	return 1;
}
