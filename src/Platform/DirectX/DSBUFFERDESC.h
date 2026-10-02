#ifndef LEMBALL_PLATFORM_DIRECTX_DSBUFFERDESC_H
#define LEMBALL_PLATFORM_DIRECTX_DSBUFFERDESC_H

struct tWAVEFORMATEX;

#define DSBCAPS_PRIMARYBUFFER 0x00000001

// SIZE 0x14
struct DSBUFFERDESC {
	unsigned long dwSize;
	unsigned long dwFlags;
	unsigned long dwBufferBytes;
	unsigned long dwReserved;
	tWAVEFORMATEX* lpwfxFormat;
};

#endif
