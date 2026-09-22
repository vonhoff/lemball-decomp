#ifndef LEMBALL_VISOS_GRAPHICS_CPVZBUFFSURFACE_H
#define LEMBALL_VISOS_GRAPHICS_CPVZBUFFSURFACE_H

#include "CPvGdiBitmap.h" // complete type
#include "CPvSurface.h"   // complete type

class CZBuffScroll;
class CZBuffClear;

// SIZE 0x54
// VTABLE: LEMBALL 0x00499618 CPvZBuffSurface
// VTABLE: LEMBALL 0x004995c8 CPvSurface
class CPvZBuffSurface : public virtual CPvSurface {
public:
	CPvZBuffSurface();
	virtual void Blit(CZBuffScroll* p_arg0) = 0; // vtable+0x04
	virtual void Blit(CZBuffClear* p_arg0) = 0;  // vtable+0x00
	virtual bool HasZBuff();                     // vtable+0x40
	void AllocateZBuff();
	void EnableZBuff(int p_enabled);
	void FreeZBuff();
	void ResizeZBuff();
	~CPvZBuffSurface();

	friend class CSurface;

private:
	CPvGdiBitmap m_bitmap;            // 0x08
	unsigned int m_enabled;           // 0x48
	unsigned short* m_buffer;         // 0x4c
	unsigned short m_allocatedWidth;  // 0x50
	unsigned short m_allocatedHeight; // 0x52
};

#endif
