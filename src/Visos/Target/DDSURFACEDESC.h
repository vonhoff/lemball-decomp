#ifndef LEMBALL_VISOS_TARGET_DDSURFACEDESC_H
#define LEMBALL_VISOS_TARGET_DDSURFACEDESC_H

// DirectDraw 1 descriptor ABI, also preserved in the Windows SDK ddraw.h.
// Only fields used by the reconstructed surface operations are named here.
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

#endif
