#ifndef LEMBALL_VISOS_RESOURCES_RESBITMAP_H
#define LEMBALL_VISOS_RESOURCES_RESBITMAP_H

#include "../../Common.h"
#include "ResRaster.h"

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
class ResBitmap : public ResRaster {
public:
	inline ResBitmap(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	inline ResBitmap() {}
	static ResBitmap* Load(unsigned int p_resourceId);
	virtual void SetHeader(); // vtable+0x08
	virtual void SetType();   // vtable+0x34

	friend class BaseFrontendDrawer;
	friend class BitmapRes;
	friend class Surface;

private:
	unsigned char m_depth; // 0x4c
	unsigned char m_flags; // 0x4d
	short m_pad0x4e;       // 0x4e
	int m_rendered;        // 0x50
};

// 68K 0x101162f6 __dt__10CResBITMAPFv
// SYNTHETIC: LEMBALL 0x0045eb80
// ResBitmap::`scalar deleting destructor'

#endif
