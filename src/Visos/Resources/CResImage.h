#ifndef LEMBALL_VISOS_RESOURCES_CRESIMAGE_H
#define LEMBALL_VISOS_RESOURCES_CRESIMAGE_H

#include "CResRaster.h"

// SIZE 0x54
// VTABLE: LEMBALL 0x00498ce0
class CResImage : public CResRaster {
public:
	CResImage(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	static CResImage* Load(unsigned int p_resourceId);
	virtual void SetHeader(); // vtable+0x08
	virtual void SetType();   // vtable+0x34

private:
	unsigned char m_depth;     // 0x4c
	unsigned char m_flags;     // 0x4d
	char m_unknown0x4e[2];     // 0x4e
	unsigned int m_imageState; // 0x50
};

// SYNTHETIC: LEMBALL 0x0045eb50
// CResImage::`scalar deleting destructor'

#endif
