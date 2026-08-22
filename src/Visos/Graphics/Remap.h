#ifndef LEMBALL_SCAFFOLD_VISOS_GRAPHICS_REMAP_H
#define LEMBALL_SCAFFOLD_VISOS_GRAPHICS_REMAP_H

#include "../../Common.h"
#include "BaseRemap.h" // complete type

// SIZE 0x08
class Remap : public BaseRemap {
public:
	Remap(unsigned long p_arg0, unsigned char* p_arg1, ePaletteTypes p_arg2);
};

#endif
