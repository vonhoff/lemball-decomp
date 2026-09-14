#ifndef LEMBALL_VISOS_RESOURCES_RESIMAGE_H
#define LEMBALL_VISOS_RESOURCES_RESIMAGE_H

#include "../../Common.h"
#include "ResRaster.h" // complete type

// SIZE 0x54
// VTABLE: LEMBALL 0x00498ce0
class ResImage : public ResRaster {
public:
	virtual void SetHeader(); // vtable+0x08
	virtual void SetType();   // vtable+0x34

private:
	unsigned char m_depth;      // 0x4c
	unsigned char m_flags;      // 0x4d
	undefined m_unknown0x4e[2]; // 0x4e
	unsigned int m_imageState;  // 0x50
};

#endif
