#ifndef LEMBALL_VISOS_GRAPHICS_CREMAP_H
#define LEMBALL_VISOS_GRAPHICS_CREMAP_H

#include "CBaseRemap.h" // complete type

// SIZE 0x08
class CRemap : public CBaseRemap {
public:
	CRemap(unsigned long p_arg0, unsigned char* p_arg1, ePaletteTypes p_arg2);
};

#endif
