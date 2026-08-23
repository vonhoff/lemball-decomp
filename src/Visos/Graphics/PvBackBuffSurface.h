#ifndef LEMBALL_VISOS_GRAPHICS_PVBACKBUFFSURFACE_H
#define LEMBALL_VISOS_GRAPHICS_PVBACKBUFFSURFACE_H

#include "../../Common.h"
#include "PvGdiBitmap.h" // complete type

// SIZE 0x9c
// VTABLE: LEMBALL 0x004995b0 primary abstract surface
// VTABLE: LEMBALL 0x00499560 PvSurface virtual base at +0x58
class PvBackBuffSurface {
public:
	PvBackBuffSurface();
	virtual void Blit(CopyToBackBuff* p_arg0);       // vtable+0x00
	virtual void Blit(CopyColourToBackBuff* p_arg0); // vtable+0x04
	virtual bool HasBackBuff();                      // vtable+0x3c
	void AllocateBackBuff();
	void EnableBackBuff(unsigned char p_enabled);
	void FreeBackBuff();
	void ResizeBackBuff();
	~PvBackBuffSurface();

private:
	void* m_virtualBaseTable;         // 0x04
	PvGdiBitmap m_bitmap;             // 0x08
	unsigned int m_enabled;           // 0x48
	unsigned char* m_buffer;          // 0x4c
	unsigned short m_allocatedWidth;  // 0x50
	unsigned short m_allocatedHeight; // 0x52
	int m_virtualBaseDisplacement;    // 0x54
};

#endif
