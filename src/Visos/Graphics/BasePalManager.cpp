#include "BasePalManager.h"

BasePalManager::BasePalManager()
{
}

// 68K 0x10200c72 __ct__15CBasePalManagerFi
// STUB: LEMBALL 0x0046acd0
BasePalManager::BasePalManager(int p_arg0)
{
}

// 68K 0x10200cd2 __dt__15CBasePalManagerFv
// STUB: LEMBALL 0x0046ad10
BasePalManager::~BasePalManager()
{
}

// 68K 0x10200d5a RegisterRemap__15CBasePalManagerFUlPUc13ePaletteTypes
// STUB: LEMBALL 0x0046ad70
BaseRemap* BasePalManager::RegisterRemap(unsigned int p_resourceId, unsigned char* p_mapping, ePaletteTypes p_type)
{
	return 0;
}

// 68K 0x10200df0 UnRegisterRemap__15CBasePalManagerFP6CRemap
// STUB: LEMBALL 0x0046add0
void BasePalManager::UnRegisterRemap(BaseRemap* p_remap)
{
}

// GLOBAL: LEMBALL 0x004a2000
BasePalManager* g_pBasePalManager;
