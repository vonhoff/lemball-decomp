#ifndef LEMBALL_PLATFORM_DIRECTX_DDSURFACEDESC_H
#define LEMBALL_PLATFORM_DIRECTX_DDSURFACEDESC_H

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
