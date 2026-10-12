#ifndef LEMBALL_VISOS_GRAPHICS_CBASEPALMANAGER_H
#define LEMBALL_VISOS_GRAPHICS_CBASEPALMANAGER_H

#include "CBaseRemap.h"

class CRemap;
class CPaletteManager;

// SIZE 0x0c
class CBasePalManager {
public:
	CBasePalManager(int p_capacity);
	CRemap* RegisterRemap(unsigned long p_resourceId, unsigned char* p_mapping, ePaletteTypes p_type);
	void UnRegisterRemap(CRemap* p_remap);
	~CBasePalManager();

private:
	CRemap** m_remaps; // 0x00
	int m_capacity;    // 0x04
	int m_remapCount;  // 0x08
};

extern CPaletteManager* g_pBasePalManager;
#endif
