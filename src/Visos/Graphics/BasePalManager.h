#ifndef LEMBALL_VISOS_GRAPHICS_BASEPALMANAGER_H
#define LEMBALL_VISOS_GRAPHICS_BASEPALMANAGER_H

#include "../../Common.h"

// SIZE 0x0c
class BasePalManager {
public:
	BasePalManager(int p_arg0);
	BaseRemap* RegisterRemap(unsigned int p_resourceId, unsigned char* p_mapping, ePaletteTypes p_type);
	void UnRegisterRemap(BaseRemap* p_remap);
	~BasePalManager();
	BasePalManager();

private:
	BaseRemap** m_remaps; // 0x00
	int m_capacity;       // 0x04
	int m_remapCount;     // 0x08
};

extern BasePalManager* g_pBasePalManager;
#endif
