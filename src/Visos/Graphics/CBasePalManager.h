#ifndef LEMBALL_VISOS_GRAPHICS_CBASEPALMANAGER_H
#define LEMBALL_VISOS_GRAPHICS_CBASEPALMANAGER_H

#include "CBaseRemap.h"

// SIZE 0x0c
class CBasePalManager {
public:
	CBasePalManager(int p_arg0);
	CBaseRemap* RegisterRemap(unsigned int p_resourceId, unsigned char* p_mapping, ePaletteTypes p_type);
	void UnRegisterRemap(CBaseRemap* p_remap);
	~CBasePalManager();
	CBasePalManager();

private:
	CBaseRemap** m_remaps; // 0x00
	int m_capacity;        // 0x04
	int m_remapCount;      // 0x08
};

extern CBasePalManager* g_pBasePalManager;
#endif
