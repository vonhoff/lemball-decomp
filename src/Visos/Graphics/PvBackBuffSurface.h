#ifndef LEMBALL_VISOS_GRAPHICS_PVBACKBUFFSURFACE_H
#define LEMBALL_VISOS_GRAPHICS_PVBACKBUFFSURFACE_H

#include "../../Common.h"
#include "PvGdiBitmap.h" // complete type
#include "PvSurface.h"   // complete type

// SIZE 0x54
// VTABLE: LEMBALL 0x004995b0 primary abstract surface
// VTABLE: LEMBALL 0x00499560 PvSurface virtual base at +0x58
class PvBackBuffSurface : public virtual PvSurface {
public:
	PvBackBuffSurface();
	virtual void Blit(CopyToBackBuff* p_arg0) = 0;               // vtable+0x04
	virtual void Blit(CopyColourToBackBuff* p_arg0) = 0;         // vtable+0x00
	virtual void CopyBackBuffToScreen(const VsRect& p_arg0) = 0; // vtable+0x08
	virtual bool HasBackBuff();                                  // vtable+0x3c
	void AllocateBackBuff();
	void EnableBackBuff(unsigned int p_enabled);
	void FreeBackBuff();
	void ResizeBackBuff();
	~PvBackBuffSurface();
	friend class Surface;

private:
	PvGdiBitmap m_bitmap;             // 0x08
	unsigned int m_enabled;           // 0x48
	unsigned char* m_buffer;          // 0x4c
	unsigned short m_allocatedWidth;  // 0x50
	unsigned short m_allocatedHeight; // 0x52
};

#endif
