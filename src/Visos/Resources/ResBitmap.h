#ifndef LEMBALL_VISOS_RESOURCES_RESBITMAP_H
#define LEMBALL_VISOS_RESOURCES_RESBITMAP_H

#include "../../Common.h"
#include "ResBase.h" // complete type

// SIZE 0x0c
struct BitmapHeader {
	unsigned int m_width;
	unsigned short m_height;
	unsigned short m_pad0x06;
	unsigned char m_depth;
	unsigned char m_flags;
};

// SIZE 0x4c
// VTABLE: LEMBALL 0x00498ab0
class ResRaster : public ResBase {
public:
	inline ResRaster()
	{
		m_y = 0;
		m_x = 0;
	}
	inline virtual ~ResRaster() {}

	friend class BaseFrontendDrawer;
	friend class MainOptions1Drawer;
	friend class MainOptions2Drawer;
	friend class Surface;
	friend class AnimsManager;
	friend class CdLoadAnim;

protected:
	short m_x; // 0x48
	short m_y; // 0x4a
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
	virtual ~ResBitmap();     // vtable+0x00

	friend class BaseFrontendDrawer;
	friend class BitmapRes;
	friend class Surface;

private:
	unsigned char m_depth;       // 0x4c
	unsigned char m_flags;       // 0x4d
	short m_pad0x4e;             // 0x4e
	int m_rendered;              // 0x50
};

// SYNTHETIC: LEMBALL 0x0045eb80
// ResBitmap::`scalar deleting destructor'


#endif

