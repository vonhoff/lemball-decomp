#ifndef LEMBALL_VISOS_GRAPHICS_CPVZBUFFSURFACE_H
#define LEMBALL_VISOS_GRAPHICS_CPVZBUFFSURFACE_H

#include "CPVGDIBitmap.h"
#include "CPVSurface.h"

class CZBuffScroll;
class CZBuffClear;

// SIZE 0x54
// VTABLE: LEMBALL 0x00499618 CPVZBuffSurface
// VTABLE: LEMBALL 0x004995c8 CPVSurface
class CPVZBuffSurface : public virtual CPVSurface {
public:
	CPVZBuffSurface();
	virtual void Blit(CZBuffScroll* p_arg0) = 0; // vtable+0x04
	virtual void Blit(CZBuffClear* p_arg0) = 0;  // vtable+0x00
	virtual bool HasZBuff();                     // vtable+0x40
	void AllocateZBuff();
	void EnableZBuff(int p_enabled);
	void FreeZBuff();
	void ResizeZBuff();
	~CPVZBuffSurface();

	friend class CSurface;

private:
	CPVGDIBitmap m_bitmap;            // 0x08
	unsigned int m_enabled;           // 0x48
	unsigned short* m_buffer;         // 0x4c
	unsigned short m_allocatedWidth;  // 0x50
	unsigned short m_allocatedHeight; // 0x52
};

// GLOBAL: LEMBALL 0x004995c0
// CPVZBuffSurface::`vbtable'

#endif
