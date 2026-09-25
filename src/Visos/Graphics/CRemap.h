#ifndef LEMBALL_VISOS_GRAPHICS_CREMAP_H
#define LEMBALL_VISOS_GRAPHICS_CREMAP_H

#include "CBaseRemap.h"

// SIZE 0x08
class CRemap : public CBaseRemap {
public:
	CRemap(unsigned long p_resourceId, unsigned char* p_mapping, ePaletteTypes p_type);
};

#endif
