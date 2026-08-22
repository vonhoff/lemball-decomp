#ifndef LEMBALL_VISOS_GRAPHICS_PVZBUFFSURFACE_H
#define LEMBALL_VISOS_GRAPHICS_PVZBUFFSURFACE_H

#include "../../Common.h"
#include "PvGdiBitmap.h" // complete type

// SIZE 0x9c
// VTABLE: LEMBALL 0x00499618 primary abstract surface
// VTABLE: LEMBALL 0x004995c8 PvSurface virtual base at +0x58
class PvZBuffSurface {
public:
	PvZBuffSurface();
	virtual bool HasZBuff(); // vtable+0x40
	void AllocateZBuff();
	void EnableZBuff(unsigned char p_enabled);
	void FreeZBuff();
	void ResizeZBuff();
	~PvZBuffSurface();

private:
	void* m_virtualBaseTable;         // 0x04
	PvGdiBitmap m_bitmap;             // 0x08
	unsigned int m_enabled;           // 0x48
	unsigned short* m_buffer;         // 0x4c
	unsigned short m_allocatedWidth;  // 0x50
	unsigned short m_allocatedHeight; // 0x52
	int m_virtualBaseDisplacement;    // 0x54
};

#endif
