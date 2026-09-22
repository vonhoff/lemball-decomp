#ifndef LEMBALL_VISOS_GRAPHICS_CPVBACKBUFFSURFACE_H
#define LEMBALL_VISOS_GRAPHICS_CPVBACKBUFFSURFACE_H

#include "CPvGdiBitmap.h" // complete type
#include "CPvSurface.h"   // complete type
class CVsRect;

class CCopyToBackBuff;
class CCopyColourToBackBuff;

// SIZE 0x54
// VTABLE: LEMBALL 0x004995b0 CPvBackBuffSurface
// VTABLE: LEMBALL 0x00499560 CPvSurface
class CPvBackBuffSurface : public virtual CPvSurface {
public:
	CPvBackBuffSurface();
	virtual void Blit(CCopyToBackBuff* p_arg0) = 0;               // vtable+0x04
	virtual void Blit(CCopyColourToBackBuff* p_arg0) = 0;         // vtable+0x00
	virtual void CopyBackBuffToScreen(const CVsRect& p_arg0) = 0; // vtable+0x08
	virtual bool HasBackBuff();                                   // vtable+0x3c
	void AllocateBackBuff();
	void EnableBackBuff(unsigned int p_enabled);
	void FreeBackBuff();
	void ResizeBackBuff();
	~CPvBackBuffSurface();
	friend class CSurface;

private:
	CPvGdiBitmap m_bitmap;            // 0x08
	unsigned int m_enabled;           // 0x48
	unsigned char* m_buffer;          // 0x4c
	unsigned short m_allocatedWidth;  // 0x50
	unsigned short m_allocatedHeight; // 0x52
};

#endif
