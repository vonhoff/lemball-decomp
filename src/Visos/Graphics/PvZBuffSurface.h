#ifndef LEMBALL_VISOS_GRAPHICS_PVZBUFFSURFACE_H
#define LEMBALL_VISOS_GRAPHICS_PVZBUFFSURFACE_H

#include "../../Common.h"
#include "PvGdiBitmap.h" // complete type
#include "PvSurface.h"   // complete type

// SIZE 0x54
// VTABLE: LEMBALL 0x00499618 PvZBuffSurface
// VTABLE: LEMBALL 0x004995c8 PvSurface
class PvZBuffSurface : public virtual PvSurface {
public:
	PvZBuffSurface();
	virtual void Blit(ZBuffClear* p_arg0) = 0;  // vtable+0x00
	virtual void Blit(ZBuffScroll* p_arg0) = 0; // vtable+0x04
	virtual bool HasZBuff();                    // vtable+0x40
	void AllocateZBuff();
	void EnableZBuff(unsigned char p_enabled);
	void FreeZBuff();
	void ResizeZBuff();
	~PvZBuffSurface();

	friend class Surface;

private:
	PvGdiBitmap m_bitmap;             // 0x08
	unsigned int m_enabled;           // 0x48
	unsigned short* m_buffer;         // 0x4c
	unsigned short m_allocatedWidth;  // 0x50
	unsigned short m_allocatedHeight; // 0x52
};

#endif
