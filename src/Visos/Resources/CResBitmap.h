#ifndef LEMBALL_VISOS_RESOURCES_CRESBITMAP_H
#define LEMBALL_VISOS_RESOURCES_CRESBITMAP_H

#include "CResRaster.h"

// SIZE 0x0c
struct BitmapHeader {
	unsigned int m_width;
	unsigned short m_height;
	unsigned short m_pad0x06;
	unsigned char m_depth;
	unsigned char m_flags;
};

// SIZE 0x54
// VTABLE: LEMBALL 0x00498d20
class CResBitmap : public CResRaster {
public:
	inline CResBitmap(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	inline CResBitmap() {}
	static CResBitmap* Load(unsigned int p_resourceId);
	virtual void SetHeader(); // vtable+0x08
	virtual void SetType();   // vtable+0x34

	friend class CBaseFrontendDrawer;
	friend class CBitmapRes;
	friend class CSurface;

private:
	unsigned char m_depth; // 0x4c
	unsigned char m_flags; // 0x4d
	short m_pad0x4e;       // 0x4e
	int m_rendered;        // 0x50
};

// SYNTHETIC: LEMBALL 0x0045eb80
// CResBitmap::`scalar deleting destructor'

#endif
