#include "BasePalManager.h"
#include "BaseRemap.h"

#include <new.h>

BasePalManager::BasePalManager()
{
}

// 68K 0x10200c72 __ct__15CBasePalManagerFi
// FUNCTION: LEMBALL 0x0046acd0
BasePalManager::BasePalManager(int p_arg0)
{
	int i;

	m_capacity = p_arg0;
	m_remapCount = 0;
	m_remaps = (BaseRemap**) operator new(p_arg0 * sizeof(BaseRemap*));
	i = 0;
	if (0 < m_capacity) {
		do {
			m_remaps[i] = 0;
			i = i + 1;
		} while (i < m_capacity);
	}
}

// 68K 0x10200cd2 __dt__15CBasePalManagerFv
// FUNCTION: LEMBALL 0x0046ad10
BasePalManager::~BasePalManager()
{
	int i;
	BaseRemap* remap;

	if (m_remaps != 0 && 0 < m_remapCount) {
		i = 0;
		do {
			remap = m_remaps[i];
			if (remap != 0) {
				remap->~BaseRemap();
				operator delete(remap);
			}
			m_remaps[i] = 0;
			i = i + 1;
		} while (i < m_remapCount);
	}
	if (m_remaps != 0) {
		operator delete(m_remaps);
	}
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
BasePalManager* g_pBasePalManager = 0;
