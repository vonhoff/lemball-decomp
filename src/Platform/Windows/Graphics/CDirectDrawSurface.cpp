#include "CDirectDrawSurface.h"

#include "Engine/Streams/CVSOStream.h"
#include "DirectDrawError.h"
#include "Platform/Windows/DirectX/DirectDraw.h"

// FUNCTION: LEMBALL 0x00457310
DDSURFACEDESC* CDirectDrawSurface::RefreshDescription()
{
	long result = m_surface->GetSurfaceDesc(&m_surfaceDescription);
	if (result != 0) {
		*g_pErrorOutput << "Direct Draw Surface Get Description failed: "
						<< FormatUnknownDirectDrawError(result & DIRECT_DRAW_ERROR_CODE_MASK) << "\n";
		return NULL;
	}
	return &m_surfaceDescription;
}

// FUNCTION: LEMBALL 0x00457360
unsigned char* CDirectDrawSurface::GetBits()
{
	if (m_bits == NULL) {
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
	while ((result = m_surface->Lock(NULL, &m_surfaceDescription, 0, NULL)) != 0) {
		if (result == (long) 0x887601c2) {
			return false;
		}
	}
	m_bits = (unsigned char*) m_surfaceDescription.lpSurface;
	m_width = m_surfaceDescription.lPitch;
	return true;
}

// FUNCTION: LEMBALL 0x004573e0
bool CDirectDrawSurface::Unlock()
{
	long result;
	while ((result = m_surface->Unlock(m_bits)) != 0) {
		if (result == (long) 0x887601c2) {
			return false;
		}
	}
	return true;
}
