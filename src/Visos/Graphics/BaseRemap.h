#ifndef LEMBALL_SCAFFOLD_VISOS_GRAPHICS_BASEREMAP_H
#define LEMBALL_SCAFFOLD_VISOS_GRAPHICS_BASEREMAP_H

#include "../../Common.h"

// SIZE 0x08
class BaseRemap {
public:
	BaseRemap(unsigned long p_arg0, unsigned char* p_arg1, ePaletteTypes p_arg2);
	void CalculateGreyScale();
	void MapRemap(const unsigned char* p_mapping);
	~BaseRemap();
	BaseRemap();

private:
	unsigned char* m_remap;  // 0x00
	void* m_paletteResource; // 0x04
};

#endif
