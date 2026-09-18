#ifndef LEMBALL_VISOS_RESOURCES_RESIMAGE_H
#define LEMBALL_VISOS_RESOURCES_RESIMAGE_H

#include "ResRaster.h" // complete type

// SIZE 0x54
// VTABLE: LEMBALL 0x00498ce0
class ResImage : public ResRaster {
public:
	ResImage(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	static ResImage* Load(unsigned int p_resourceId);
	virtual void SetHeader(); // vtable+0x08
	virtual void SetType();   // vtable+0x34

private:
	unsigned char m_depth;     // 0x4c
	unsigned char m_flags;     // 0x4d
	char m_unknown0x4e[2];     // 0x4e
	unsigned int m_imageState; // 0x50
};

// SYNTHETIC: LEMBALL 0x0045eb50
// ResImage::`scalar deleting destructor'

#endif
